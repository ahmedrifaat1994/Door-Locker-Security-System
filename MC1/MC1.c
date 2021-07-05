/*
 * MC1.c
 *
 *  Created on: 8 Jun 2021
 *      Author: A.Rifaat
 */
#define F_CPU 8000000UL
#include"lcd.h"
#include"keypad.h"
#include"uart.h"
#include"timers.h"
#include"interrupt.h"

uint8 g_firstTime = 0; /*If set to 0 means that the user is using this program for the first time or changing password*/
uint8 g_password[5]; /*Saves the entered password*/
uint8 g_overflow = 0; /*For delay function to save the amount of interrupts happened*/

/******************Delay time equation*****************
 * F_CPU = 8M, and pre-scaler = 64                    *
 * then 8M / 64 = 125000, so 125000 / 65535 = 1.9.    *
 * so every 2 interrupts count 1 second               *
 * ****************************************************/

/*Function for delay*/
void delay(void){
	g_overflow++;
}

int main(){
	/*********************************************Variables*************************************************************
	 * passCount, we use it to check the number of entered numbers of password.										   *
	 * re_enter, we use it to limit the operation of entering password to 2 times only.								   *
	 * checkPass[5], we save the password here to check with the first attempt password.							   *
	 * enterAgain, set it to 1 after the first attempt to enter password to not let user see the first screen again.   *
	 * flag, we set it to 1 after entering the password correct twice.												   *
	 * i, variable for loops.																						   *
	 * option, variable for the option user wants (+, -).															   *
	 * thief, if incremented to be 3, activate buzzer. 																   *
	 * *****************************************************************************************************************/
	uint8 passCount = 0, re_enter = 0, checkPass[5], enterAgain = 0, flag = 0, i = 0;
	uint8 option = 0, thief = 0;

	LCD_init(); /*Initialise LCD*/
	UART_init();/*Initialise UART*/

	/*Configuration values for timer1*/
	/********Timer1 Configurations*****
	 * 1- Normal mode                 *
	 * 2- 64 pre-scaler              *
	 * 3- Initial value = 0           *
	 * 4- Output Mode Disabled        *
	 * 5- Interrupt Enabled           *
	 **********************************/
	Timer16ConfigType timer1_Config = {OVERFLOW_16,F_CPU_64,0,0,DISCONNECT,INT_ENABLE};

	TIMER1_OVF_interrupt(delay); /*Call back function for timer1 overflow mode*/

	/*get the value of re-enter, to see if the user registered or not
	 * if registered prevent him from entering the register page*/
	UART_sendByte('I'); /*Send a signal to MMC2 to get re_enter value from EEPROM*/
	_delay_ms(10);
	re_enter = UART_recieveByte(); /*Receive re_enter value*/

	/*if re_enter not equal 2 means user didn't register yet*/
	if(re_enter != 2){
		re_enter = 0; /*if not equal to 2 assign it to zero, because there might be garbage values returned from EEPROM*/
	}

	while(1){

		/***************************************************Register Page*******************************************************/
		while(re_enter < 2){
			/*tell user to enter password of 5 numbers*/
			if(enterAgain == 0)
			{
				LCD_clearScreen(); /*Clear screen*/
				LCD_displayString("Register:-"); /*write "Enter Password" on screen*/
				LCD_displayStringRowColumn(1, 0, "5-numbers:");
				LCD_goToRowColumn(1,11); /*Go to the second Row*/
				/*the program executes enter password code block twice, so prevent the program from entering this section again*/
				enterAgain = 1;
			}

			/*Enter password*/
			if(g_firstTime == 0){
				/*get password from user to save it*/
				i = 0;
				while(passCount < 5){
					if(KeyPad_getPressedKey()){
						g_password[i] = KeyPad_getPressedKey(); /*save password*/
						LCD_displayCharacter('*'); /*write '*' for every entered number*/
						_delay_ms(500);
						i++; /*Incrementing password array*/
						passCount++; /*If equal 5 mean the user enter 5 numbers*/
					}
				}
				/*tell the user to re-enter the password for checking*/
				if(re_enter == 0){
					LCD_clearScreen(); /*clear screen*/
					LCD_displayString("Re-enter:"); /*write "Re-enter:" on screen*/
					LCD_goToRowColumn(1,0); /*Go to the second Row*/
					passCount = 0; /*reset enter password code block*/
				}

				/*let the user enter the password twice only*/
				if(re_enter < 2){
					re_enter++;
				}

				/*if the user re-entered the password, save the first attempt to a new variable*/
				if(re_enter == 1){
					for(i = 0; i < 5; i++){
						checkPass[i] = g_password[i];
						g_password[i] = 0;
					}
					i = 0;
				}

				/*if the user re-entered the password, check if the two passwords are equal */
				if(re_enter == 2){
					/*check if two password match, if not equal make the user try again*/
					for(i = 0; i < 5; i++){
						/*this section will happen if password not matching*/
						if(checkPass[i] != g_password[i]){
							re_enter = passCount = enterAgain = i = 0; /*reset every thing to try again*/
							LCD_clearScreen(); /*clear screen*/
							LCD_displayString("Not Matching"); /*tell him the password is not matching*/
							_delay_ms(1000); /*show the message for 1 second*/
							break;
						}
					}

					/*if the password is correct tell the user that*/
					if (re_enter == 2){
						LCD_clearScreen(); /*clear screen*/
						LCD_displayString("Correct"); /*show the password is matching*/
						_delay_ms(1000); /*show the message for 1 second*/
						LCD_clearScreen(); /*clear screen*/
						flag = 1; /*prevent the program from entering register page again, only in change password mode*/

						/*save re_enter value to keep the user at main page,
						 * prevent him from entering the password every time he restarts the program*/
						UART_sendByte('R'); /*send a signal to MC2 to save the next sent data*/
						UART_sendByte(re_enter); /*save this data in EEPROM*/
						_delay_ms(10);/*give time to EEPROM to write data*/                   /*<--------------< The problem was here*/
					}
				}
		}
			if(flag == 1){
				/*if he entered the two attempts right prevent him from entering this section again*/
				g_firstTime = 1;

				/*Send the password to MC2 to save it to EEPROM*/
				for(i = 0; i < 5; i++){
					if(i == 0){
						UART_sendByte('@'); /*send a signal to MC2 to save the next sent data*/
					}
					UART_sendByte(g_password[i]); /*send password to MC2 to save it in EEPROM*/
				}

				LCD_clearScreen(); /*clear screen*/
				LCD_displayString("Saved"); /*show the password is saved*/
				_delay_ms(1000); /*show message for 1 second*/
			}
		}

		/***************************************************Main Options Page*******************************************************/

		while(re_enter == 2){

			while(((option != '+') || (option != '-')) && (re_enter == 2)){
				/*show options "+: Open Door" and "-: Change Pass"*/
				if((option == 0) && (re_enter == 2)){
					LCD_clearScreen(); /*clear screen*/

					/*show the options available*/
					LCD_displayStringRowColumn(0, 0, "+: Open Door");
					LCD_displayStringRowColumn(1, 0, "-: Change Pass");
				}

				option = KeyPad_getPressedKey(); /*get the chose option from user*/
				_delay_ms(500);

				/**********************************************Open Door Page********************************************************/
				/*if the user choose open door option*/
				while(option == '+'){
					LCD_clearScreen(); /*clear screen*/
					LCD_displayString("Enter password:"); /*show message*/
					LCD_goToRowColumn(1,0);

					/*get password from user*/
					passCount = 0, i = 0;

					while(passCount < 5){
						if(KeyPad_getPressedKey()){
							checkPass[i] = KeyPad_getPressedKey();
							LCD_displayCharacter('*');
							_delay_ms(500);
							i++;
							passCount++;
						}
					}

					/*get password from EEPROM and check it*/
					for(i = 0; i < 5; i++){

						if(i == 0){
							UART_sendByte('S'); /*tell MC2 to send the saved password*/
						}

						/*check two passwords*/
						if(checkPass[i] != UART_recieveByte()){
							LCD_clearScreen();
							LCD_displayString("Not Matching.");
							thief++;
							_delay_ms(1000);
							break;
						}
					}
					/*if the user entered the password right, open the door*/
					if(i == 5){
						LCD_clearScreen();
						LCD_displayString("Correct");
						LCD_displayStringRowColumn(1, 0, "Open Door.");
						UART_sendByte('#'); /*send to MC2 signal to open door*/

						/*synchronise the LCD with MC2 timer*/
						EnableGlobalInterrupt(); /*enable global interrupt*/
						TIMER1_init(&timer1_Config); /*initialise timer1*/
						while(g_overflow != 114/4); /*wait 15 seconds*/
						LCD_displayStringRowColumn(1, 0, "Close Door"); /**close door*/
						while(g_overflow != 114/2); /*wait 15 seconds*/
						TIMER1_deinit(); /*disable timer1*/
						DisableGlobalInterrupt(); /*disable global interrupt*/
						g_overflow = 0;

						/*show to the user after finishing opening and closing the door*/
						LCD_clearScreen();
						LCD_displayString("Enter: Home Page");

						/*if the user wanted to go to the home page, reset variables of main page only not the whole program*/
						while(option == '+'){
							if(KeyPad_getPressedKey() == 13){
								option = i = thief = passCount = 0; /*go to main option page*/
							}
						}
					}
					/*if the user entered the password wrong 3 times*/
					if(thief == 3){
						/*Activate Buzzer*/
						UART_sendByte('B');/*send signal to MC2 to activate buzzer*/
						LCD_displayStringRowColumn(0, 0, "Calling security");
						LCD_displayStringRowColumn(1, 0, "Run");

						EnableGlobalInterrupt();/*enable global interrupt*/
						TIMER1_init(&timer1_Config);/*start timer1*/
						while(g_overflow != 114);/*wait for 1 minute*/
						TIMER1_deinit();/*disable timer1*/
						DisableGlobalInterrupt();/*disable global interrupt*/
						g_overflow = 0;

						/*reset variables of main page only not the whole program*/
						option = i = thief = passCount = 0;
					}
				}
				/*************************************************Change Password Page*************************************************/
				while(option == '-'){
					LCD_clearScreen(); /*clear screen*/
					LCD_displayString("Old password:"); /*show message*/
					LCD_goToRowColumn(1,0);
					passCount = i = 0;

					/*Get password*/
					while(passCount < 5){
						if(KeyPad_getPressedKey()){
							checkPass[i] = KeyPad_getPressedKey();
							LCD_displayCharacter('*');
							_delay_ms(500);
							i++;
							passCount++;
						}
					}

					/*check password*/
					for(i = 0; i < 5; i++){
						if(i == 0){
							UART_sendByte('S'); /*send signal to MC2 to get the saved password*/
						}

						/*receive password and check it*/
						if(checkPass[i] != UART_recieveByte()){
							LCD_clearScreen();
							LCD_displayString("Not Matching.");
							thief++;
							_delay_ms(1000);
							break;
						}
					}

					/*if the user wanted to change password and entered old password right, reset every thing*/
					if(i == 5){
						g_firstTime = passCount = re_enter = enterAgain = flag = i = option = thief = 0;
						UART_sendByte('R'); /*send signal to MC2 to reset re_enter value in address 0x0000*/
						UART_sendByte(0xFF); /*send the new value of re_enter*/
						_delay_ms(10);
					}

					/*if the user entered the password wrong 3 times*/
					if(thief == 3){
						/*Activate Buzzer*/
						UART_sendByte('B'); /*send signal to MC2 to activate buzzer*/
						LCD_displayStringRowColumn(0, 0, "Calling security");
						LCD_displayStringRowColumn(1, 0, "Run");

						EnableGlobalInterrupt(); /*enable global interrupt*/
						TIMER1_init(&timer1_Config); /*start timer1*/
						while(g_overflow != 114); /*wait for 1 minute*/
						TIMER1_deinit(); /*disable timer1*/
						DisableGlobalInterrupt(); /*disable global interrupt*/
						g_overflow = 0;

						/*reset variables of main page only not the whole program*/
						option = i = thief = passCount = 0;
					}
				}
			}
		}
	}
}
