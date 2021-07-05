/*
 * MC2.c
 *
 *  Created on: 8 Jun 2021
 *      Author: A.Rifaat
 */
#define F_CPU 8000000UL

#include"uart.h"
#include"timers.h"
#include"motors.h"
#include"external_eeprom.h"
#include"interrupt.h"

uint16 address = 0x0000;
uint8 g_overflow = 0;

/******************Delay time equation*****************
 * F_CPU = 8M, and pre-scaler = 64                    *
 * then 8M / 64 = 125000, so 125000 / 65535 = 1.9.    *
 * so every 2 interrupts count 1 second               *
 * ****************************************************/

/*delay function for motors*/
void delay(void){
	g_overflow++;
	if(g_overflow == 114/4){
		TCNT1 = 0;
		MOTOR_ACW();
	}
	else if(g_overflow == 114/2){
		MOTOR_stop();
		TCNT1 = 0;
	}
}

/*delay function for buzzer*/
void buzzer_delay(void){
	g_overflow++;
}

int main(){
	uint8 status = 0, pass[5], i, read = 0;
	DDRB |= (1<<PB3); /*OC0 output for PWM*/
	DDRA = 0x03; /*Output pins for motor*/
	DDRD |= (1<<PD4); /*Buzzer pin output*/

	/*Configuration values for timer0*/
	/********Timer0 Configurations*****
	 * 1- PWM mode                    *
	 * 2- non-inverting mode          *
	 * 3- 64 pre-scaler               *
	 * 4- Initial value = 0           *
	 * 5- Compare value = 256/2       *
	 **********************************/
	Timer8ConfigType timer0_Confiq = {PWM,F_CPU_64,0,(256/2),CLEAR_NON_INVERTING};

	/*Configuration values for timer1*/
	/********Timer1 Configurations*****
	 * 1- Normal mode                 *
	 * 2- 64 pre-scaler              *
	 * 3- Initial value = 0           *
	 * 4- Output Mode Disabled        *
	 * 5- Interrupt Enabled           *
	 **********************************/
	Timer16ConfigType timer1_Config = {OVERFLOW_16,F_CPU_64,0,0,DISCONNECT,INT_ENABLE};

	TIMER0_init(&timer0_Confiq); /*Initialise Timer0*/
	UART_init(); /*Initialise UART*/
	EEPROM_init(); /*Initialise EEPROM*/

	TIMER1_OVF_interrupt(delay); /*Call back function for timer1 overflow mode*/

	while(1){
		status = UART_recieveByte();

		/*read re_enter at the start of the program*/
		if(status == 'I'){
			EEPROM_readByte(0x0000, &read);/*read value at address 0x0000 from EEPROM*/
			_delay_ms(10);
			UART_sendByte(read); /*send value to MC1*/
			status = 0;
		}

		/*Receive the password and save it to the EEPROM*/
		if(status == '@'){
			address = 0x0000;
			/*Receive password from MC1*/
			for(i = 0; i < 5; i++){
				pass[i] = UART_recieveByte();
			}

			/*save data to EEPROM starting from address 0x0001*/
			for(i = 0; i < 5; i++){
				EEPROM_writeByte(++address, pass[i]);
				_delay_ms(10);
			}
			status = 0;
		}

		/*save re_enter value to keep the user at main page, prevent him from entering the password every time he restarts the program*/
		if(status == 'R'){
			EEPROM_writeByte(0x0000, UART_recieveByte()); /*save re_enter value to address 0x0000*/
			_delay_ms(10);
			status = 0;
		}

		/*Send the saved password to MC1*/
		if(status == 'S'){
			address = 0x0000;
			for(i = 0; i < 5; i++){
				EEPROM_readByte(++address, &pass[i]); /*read saved password from EEPROM*/
				_delay_ms(10);
			}

			for(i = 0; i < 5; i++){
				UART_sendByte(pass[i]); /*send saved password to MC1*/
			}

			status = 0;

		}

		/*open door*/
		if(status == '#'){
			EnableGlobalInterrupt(); /*enable global interrupt*/
			TIMER1_init(&timer1_Config); /*start timer1*/
			MOTOR_CW(); /*start motor clock wise*/
			while(g_overflow != 114/2); /*wait 30 seconds*/
			TIMER1_deinit(); /*stop timer1*/
			DisableGlobalInterrupt(); /*disable global interrupt*/
			g_overflow = 0;
			status = 0;
		}

		/*activate buzzer*/
		if(status == 'B'){
			PORTD |= (1<<PD4); /*switch buzzer on*/

			/*delay 1 minute*/
			EnableGlobalInterrupt(); /*enable global interrupt*/
			TIMER1_OVF_interrupt(buzzer_delay);
			TIMER1_init(&timer1_Config); /*start timer1*/
			while(g_overflow != 114); /*wait 1 minute*/
			TIMER1_deinit(); /*stop timer1*/
			DisableGlobalInterrupt(); /*disable global interrupt*/
			g_overflow = 0;

			PORTD &= ~(1<<PD4); /*switch buzzer off*/
			status = 0;
		}
	}
}

