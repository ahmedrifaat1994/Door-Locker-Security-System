/*
 * timers.h
 *
 *  Created on: 9 Jun 2021
 *      Author: A.Rifaat
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*****************************************************Common Configurations*********************************************************/

typedef enum
{
	INT_DISABLE,INT_ENABLE
}intState;

typedef enum
{
	OVERFLOW,PWM,COMPARE
}timerMode;

typedef enum
{
	DISCONNECT,TOGGLE,CLEAR_NON_INVERTING,SET_INVERTING
}outputMode;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}clock;

/***********************************************Timer0,2 Configurations Structure******************************************************/

typedef struct
{	timerMode mode; /*choose the timer mode*/
	clock prescaler; /*choose the pre-scaler*/
	uint8 initialValue; /*choose initial value*/
	uint8 compareValue; /*choose compare value*/
	outputMode output; /*chose the output mode*/
	intState interrupt; /*interrupt enable/disable*/
}Timer8ConfigType;

/***********************************************Timer1 Configurations Structure********************************************************/
typedef enum{
	OVERFLOW_16,PWM_P_8,PWM_P_9,PWM_P_10,CTC_OCR,F_PWM_8,F_PWM_9,F_PWM_10,PWM_ICR_BOTTOM,PWM_OCR_BOTTOM,
	PWM_ICR_TOP,PWM_OCR_TOP,CTC_ICR,F_PWM_ICR_BOTTOM = 14,F_PWM_OCR_BOTTOM = 15
}timer16Mode;

typedef enum{
	DISABLE,ENABLE
}icu;

typedef enum{
	FALLING,RISING
}edge;

typedef struct
{	timer16Mode mode; /*choose the timer mode*/
	clock prescaler; /*choose the pre-scaler*/
	uint16 initialValue; /*choose initial value*/
	uint16 compareValue; /*choose compare value*/
	outputMode output; /*choose the output mode*/
	intState interrupt; /*Enable/disable interrupt*/
	icu icuStatus; /*enable ICU*/
	uint16 icuInit; /*ICU initialise value*/
	edge icuEdge; /*ICU Edge*/
	intState icyINT; /*ICU interrupt*/
}Timer16ConfigType;


/************************************************Timers Prototypes**********************************************************************/
void TIMER0_init(Timer8ConfigType* config_ptr); /*Initialise timer0*/
void TIMER0_deinit(void); /*Stop timer0*/
void TIMER1_init(const Timer16ConfigType* config_ptr); /*Initialise timer1*/
void TIMER1_deinit(void); /*Stop timer1*/
void TIMER2_init(Timer8ConfigType* config_ptr); /*Initialise timer2*/
void TIMER2_deinit(void); /*Stop timer2*/
void Icu_setEdgeDetectionType(const edge a_edgeType); /*Choose ICU edge*/
uint16 Icu_getInputCaptureValue(void); /*Return ICU capture value*/
void Icu_clearTimerValue(void);/*Clear timer*/
void Icu_DeInit(void); /*Stop ICU*/

/******************************************************Call Back Functions Prototype****************************************************/
void TIMER0_OVF_interrupt(void(*a_ptr)(void)); /*Timer0 OVERFLOW Interrupt Service Routine*/
void TIMER0_COMP_interrupt(void(*a_ptr)(void));/*Timer0 COMPARE Interrupt Service Routine*/
void TIMER1_OVF_interrupt(void(*a_ptr)(void)); /*Timer1 OVERFLOW Interrupt Service Routine*/
void TIMER1_COMPA_interrupt(void(*a_ptr)(void)); /*Timer1 COMPARE A Interrupt Service Routine*/
void TIMER1_COMPB_interrupt(void(*a_ptr)(void)); /*Timer1 COMPARE B Interrupt Service Routine*/
void TIMER1_CAPT_interrupt(void(*a_ptr)(void)); /*Timer1 INPUT CAPTURE UNIT Interrupt Service Routine*/
void TIMER2_OVF_interrupt(void(*a_ptr)(void)); /*Timer2 OVERFLOW Interrupt Service Routine*/
void TIMER2_COMP_interrupt(void(*a_ptr)(void)); /*Timer2 COMPARE Interrupt Service Routine*/

#endif /* TIMERS_H_ */
