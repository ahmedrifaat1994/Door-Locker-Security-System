/*
 * lcd.c
 *
 *  Created on: 22 May 2021
 *      Author: A.Rifaat
 */
#include "lcd.h"

void LCD_sendCommand(uint8 command){
	/* 1- RS = 0*/
	RS_RW_E_DDR |= (1<<RS) | (1<<RW) | (1<<E);
	DDR_PORT = 0xff;
	RS_RW_E_PORT &= ~(1<<RS);
	/*2- R/W = 0*/
	RS_RW_E_PORT &= ~(1<<RW);
	/*3- Delay 1ms*/
	_delay_ms(5);
	/*4- enable = 1*/
	RS_RW_E_PORT |= (1<<E);
	/*5- delay 1ms*/
	_delay_ms(5);
	/*6- PORTC = command*/
	OUTPUT_PORT = command;
	/*7- delay 1ms*/
	_delay_ms(5);
	/*8- disable enable = 0*/
	RS_RW_E_PORT &= ~(1<<E);
	/*9- delay 1ms*/
	_delay_ms(5);
}

void LCD_displayString(const uint8 *str){
	while(*str != '\0'){
		LCD_displayCharacter(*str);
		str++;
	}
}

void LCD_displayCharacter(uint8 data){
	/* 1- RS = 0*/
	RS_RW_E_DDR |= (1<<RS) | (1<<RW) | (1<<E);
	DDR_PORT = 0xff;
	RS_RW_E_PORT |= (1<<RS);
	/*2- R/W = 0*/
	RS_RW_E_PORT &= ~(1<<RW);
	/*3- Delay 1ms*/
	_delay_ms(5);
	/*4- enable = 1*/
	RS_RW_E_PORT |= (1<<E);
	/*5- delay 1ms*/
	_delay_ms(5);
	/*6- PORTC = command*/
	OUTPUT_PORT = data;
	/*7- delay 1ms*/
	_delay_ms(5);
	/*8- disable enable = 0*/
	RS_RW_E_PORT &= ~(1<<E);
	/*9- delay 1ms*/
	_delay_ms(5);
}

void LCD_init(void){
	RS_RW_E_DDR |= (1<<4) | (1<<5) | (1<<6);
	DDR_PORT = 0xff;
	LCD_sendCommand(0x38);
	LCD_sendCommand(0x0C);
	LCD_sendCommand(0x01);
}

void LCD_goToRowColumn(uint8 row, uint8 col){
	uint8 address;

	switch(row){
	case 0 : address = col;
	break;
	case 1 : address = col + SECOND_ROW;
	break;
	case 2 : address = col + THIRD_ROW;
	break;
	case 3 : address = col + FOURTH_ROW;
	break;
	}

	LCD_sendCommand(address | SET_CURSOR_LOCATION);
}

void LCD_displayStringRowColumn(uint8 row, uint8 col, const char* string){
	LCD_goToRowColumn(row, col);
	LCD_displayString(string);
}

void LCD_clearScreen(void){
	LCD_sendCommand(0x01);
}

void LCD_integerToString(int integer){
	char string[16];
	itoa(integer,string,10);
	LCD_displayString(string);
}
