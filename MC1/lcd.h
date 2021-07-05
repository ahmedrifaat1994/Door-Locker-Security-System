/*
 * lcd.h
 *
 *  Created on: 22 May 2021
 *      Author: A.Rifaat
 */

#ifndef LCD_H_
#define LCD_H_
#include"common_macros.h"
#include"micro_config.h"
#include"std_types.h"

/*****************************************************************/
/******************** USER CONFOGURATIONS*************************/
/*****************************************************************/
#define DDR_PORT DDRC /*CHOOSE WHICH PORT ARE PINS 0-7 CONNECTED TO*/
#define OUTPUT_PORT PORTC

#define RS_RW_E_DDR DDRD /*CHOOSE WHICH PORT ARE PINS RS, RW, E CONNECTED TO*/
#define RS_RW_E_PORT PORTD

/*DEFINE RS, RW, E PINS*/
#define RS PD4
#define RW PD5
#define E PD6
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#define SET_CURSOR_LOCATION 0x80
#define SECOND_ROW 0x40
#define THIRD_ROW 0x10
#define FOURTH_ROW 0x50


void LCD_init(void); /*Initialise LCD*/
void LCD_sendCommand(uint8 command); /*Function to send command to the LCD*/
void LCD_displayCharacter(uint8 data); /*Function to display one character*/
void LCD_displayString(const uint8 *str); /*Function to display string*/
void LCD_clearScreen(void); /*Clear the LCD*/
void LCD_goToRowColumn(uint8 row, uint8 col); /*Force the cursor to go to a specific location in the LCD*/
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char* string); /*Displays a string in a specific location*/
void LCD_integerToString(int integer); /*Takes and integer number and converts it to string*/

#endif /* LCD_H_ */
