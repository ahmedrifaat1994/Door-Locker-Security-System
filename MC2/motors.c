/*
 * motors.c
 *
 *  Created on: 10 Jun 2021
 *      Author: A.Rifaat
 */
#include"motors.h"
/*Start Motor Clock wise*/
void MOTOR_CW(void){
	PORTA = 0x01;
}
/*Start Motor Anti Clock wise*/
void MOTOR_ACW(void){
	PORTA = 0x02;
}
/*Stop Motor*/
void MOTOR_stop(void){
	PORTA = 0x00;
}
