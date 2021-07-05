/*
 * timers.c
 *
 *  Created on: 9 Jun 2021
 *      Author: A.Rifaat
 */
#include"timers.h"

/*******************************************Pointer to Functions for Interrupt Service Routine******************************************/
static volatile void (*timer0_ovf_ptr)(void)  = NULL; /*pointer to timer0 overflow function*/
static volatile void (*timer0_comp_ptr)(void)  = NULL; /*pointer to timer0 compare function*/
static volatile void (*timer1_ovf_ptr)(void)  = NULL; /*pointer to timer1 overflow function*/
static volatile void (*timer1_compa_ptr)(void)  = NULL; /*pointer to timer1 compare a function*/
static volatile void (*timer1_compb_ptr)(void)  = NULL; /*pointer to timer1 compare b function*/
static volatile void (*timer1_capt_ptr)(void)  = NULL; /*pointer to timer1 icu function*/
static volatile void (*timer2_ovf_ptr)(void)  = NULL; /*pointer to timer2 overflow function*/
static volatile void (*timer2_comp_ptr)(void)  = NULL; /*pointer to timer0 compare function*/

/**************************************************************ISR of Timers Interrupts***********************************************/
/*timer0 overflow ISR*/
ISR(TIMER0_OVF_vect){
	if(timer0_ovf_ptr != NULL){
		(*timer0_ovf_ptr)();
	}
}
/*timer0 compare ISR*/
ISR(TIMER0_COMP_vect){
	if(timer0_comp_ptr != NULL){
		(*timer0_comp_ptr)();
	}
}
/*timer1 overflow ISR*/
ISR(TIMER1_OVF_vect){
	if(timer1_ovf_ptr != NULL){
		(*timer1_ovf_ptr)();
	}
}
/*timer1 compare a ISR*/
ISR(TIMER1_COMPA_vect){
	if(timer1_compa_ptr != NULL){
		(*timer1_compa_ptr)();
	}
}
/*timer1 compare b ISR*/
ISR(TIMER1_COMPB_vect){
	if(timer1_compb_ptr != NULL){
		(*timer1_compb_ptr)();
	}
}
/*timer1 icu ISR*/
ISR(TIMER1_CAPT_vect){
	if(timer1_capt_ptr != NULL){
		(*timer1_capt_ptr)();
	}
}
/*timer2 overflow ISR*/
ISR(TIMER2_OVF_vect){
	if(timer2_ovf_ptr != NULL){
		(*timer2_ovf_ptr)();
	}
}
/*timer2 compare ISR*/
ISR(TIMER2_COMP_vect){
	if(timer2_comp_ptr != NULL){
		(*timer2_comp_ptr)();
	}
}

/***************************************************Call Back Functions For ISR***************************************************/
/*timer0 overflow Call Back Function*/
void TIMER0_OVF_interrupt(void(*a_ptr)(void)){
	timer0_ovf_ptr = a_ptr;
}
/*timer0 compare Call Back Function*/
void TIMER0_COMP_interrupt(void(*a_ptr)(void)){
	timer0_comp_ptr = a_ptr;
}
/*timer1 overflow Call Back Function*/
void TIMER1_OVF_interrupt(void(*a_ptr)(void)){
	timer1_ovf_ptr = a_ptr;
}
/*timer1 compare a Call Back Function*/
void TIMER1_COMPA_interrupt(void(*a_ptr)(void)){
	timer1_compa_ptr = a_ptr;
}
/*timer1 compare b Call Back Function*/
void TIMER1_COMPB_interrupt(void(*a_ptr)(void)){
	timer1_compb_ptr = a_ptr;
}
/*timer1 icu Call Back Function*/
void TIMER1_CAPT_interrupt(void(*a_ptr)(void)){
	timer1_capt_ptr = a_ptr;
}
/*timer2 overflow Call Back Function*/
void TIMER2_OVF_interrupt(void(*a_ptr)(void)){
	timer2_ovf_ptr = a_ptr;
}
/*timer02 compare Call Back Function*/
void TIMER2_COMP_interrupt(void(*a_ptr)(void)){
	timer2_comp_ptr = a_ptr;
}

/***********************************************************Timers Functions*********************************************************/

/***********************************************************Timer0 Initialise Function***********************************************/
void TIMER0_init(Timer8ConfigType* config_ptr){

	/*If the use choose overflow mode*/
	if((*config_ptr).mode == OVERFLOW){
		/*choose overflow*/
		TCCR0 &= ~(1<<WGM00);
		TCCR0 &= ~(1<<WGM01);
		/*choose pre-scaler*/
		TCCR0 |= (TCCR0 & 0xF8) | ((*config_ptr).prescaler);
		/*choose initial value*/
		TCNT0 = (*config_ptr).initialValue;
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK = (1<<TOIE0);
		}
	}
	/*if the use choose compare mode*/
	else if((*config_ptr).mode == COMPARE)
	{
		/*choose compare mood*/
		TCCR0 = (1<<WGM01);
		/*choose from DISCONNECT, TOGGLE, CLEAR, SET*/
		TCCR0 = (TCCR0 & 0xCF) | (((*config_ptr).output)<<4);
		/*choose pre-scaler*/
		TCCR0 |= (TCCR0 & 0xF8) | ((*config_ptr).prescaler);
		/*choose initial value*/
		TCNT0 = (*config_ptr).initialValue;
		/*choose compare value*/
		OCR0 = (*config_ptr).compareValue;
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK = (1<<OCIE0);
		}
	}
	else if((*config_ptr).mode == PWM){
		/*choose PWM mood*/
		TCCR0 |= (1<<WGM00) | (1<<WGM01);
		/*choose from DISCONNECT,NON_INVERTING,NVERTING*/
		TCCR0 = (TCCR0 & 0xCF) | (((*config_ptr).output)<<4);
		/*choose pre-scaler*/
		TCCR0 |= (TCCR0 & 0xF8) | ((*config_ptr).prescaler);
		/*choose initial value*/
		TCNT0 = (*config_ptr).initialValue;
		/*choose compare value*/
		OCR0 = (*config_ptr).compareValue;
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK = (1<<OCIE0);
		}
	}
}

/**********************************************************Timer0 Stop Function*******************************************************/
void TIMER0_deinit(void){
	/*Timer stop*/
	TCCR0 = 0;
}

/***********************************************************Timer2 Initialise Function***********************************************/
void TIMER2_init(Timer8ConfigType* config_ptr){
	/*If the use choose overflow mode*/
	if((*config_ptr).mode == OVERFLOW){
		/*choose overflow*/
		TCCR2 &= ~(1<<WGM20);
		TCCR2 &= ~(1<<WGM21);
		/*choose pre-scaler*/
		TCCR2 |= (TCCR2 & 0xF8) | ((*config_ptr).prescaler);
		/*choose initial value*/
		TCNT2 = (*config_ptr).initialValue;
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK = (1<<TOIE2);
		}
	}
	/*if the use choose compare mode*/
	else if((*config_ptr).mode == COMPARE)
	{
		/*choose compare mood*/
		TCCR2 = (1<<WGM21);
		/*choose from DISCONNECT, TOGGLE, CLEAR, SET*/
		TCCR2 = (TCCR2 & 0xCF) | (((*config_ptr).output)<<4);
		/*choose pre-scaler*/
		TCCR2 |= (TCCR2 & 0xF8) | ((*config_ptr).prescaler);
		/*choose initial value*/
		TCNT2 = (*config_ptr).initialValue;
		/*choose compare value*/
		OCR2 = (*config_ptr).compareValue;
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK = (1<<OCIE2);
		}
	}
	else if((*config_ptr).mode == PWM){
		/*choose PWM mood*/
		TCCR2 |= (1<<WGM20) | (1<<WGM21);
		/*choose from DISCONNECT,NON_INVERTING,NVERTING*/
		TCCR2 = (TCCR2 & 0xCF) | (((*config_ptr).output)<<4);
		/*choose pre-scaler*/
		TCCR2 |= (TCCR2 & 0xF8) | ((*config_ptr).prescaler);
		/*choose initial value*/
		TCNT2 = (*config_ptr).initialValue;
		/*choose compare value*/
		OCR2 = (*config_ptr).compareValue;
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK = (1<<OCIE2);
		}
	}
}

/**********************************************************Timer0 Stop Function*******************************************************/
void TIMER2_deinit(void){
	/*Timer stop*/
	TCCR2 = 0;
}

/***********************************************************Timer1 Initialise Function***********************************************/
void TIMER1_init(const Timer16ConfigType* config_ptr){
	if((*config_ptr).mode == OVERFLOW_16){
		/*choose overflow mode*/
		TCCR1A = (*config_ptr).mode;
		TCCR1B = (*config_ptr).mode;
		/*set HIGH with non-WM mode*/
		TCCR1A |= (1<<FOC1A) | (1<<FOC1B);
		/*Enable Overflow Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK |= (1<<TOIE1);
		}

		if((*config_ptr).icuStatus == ENABLE){
			/* Configure ICP1/PD6 as i/p pin */
			DDRD &= ~(1<<PD6);
			/*
			 * insert the required edge type in ICES1 bit in TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xBF) | (((*config_ptr).icuEdge)<<6);
			/* Initial Value for the input capture register */
			ICR1 = (*config_ptr).icuInit;
			/*Enable ICU Interrupt*/
			if((*config_ptr).icyINT == INT_ENABLE){
				/* Enable the Input Capture interrupt to generate an interrupt when edge is detected on ICP1/PD6 pin */
				TIMSK |= (1<<TICIE1);
			}

		}
	}
	else if(((*config_ptr).mode == PWM_P_8) || ((*config_ptr).mode == PWM_P_9) || ((*config_ptr).mode == PWM_P_10)
			|| ((*config_ptr).mode == F_PWM_8) || ((*config_ptr).mode == F_PWM_9) || ((*config_ptr).mode == F_PWM_10)
			|| ((*config_ptr).mode == PWM_ICR_BOTTOM) || ((*config_ptr).mode == PWM_OCR_BOTTOM) || ((*config_ptr).mode == PWM_ICR_TOP)
			|| ((*config_ptr).mode == PWM_OCR_TOP) || ((*config_ptr).mode == F_PWM_ICR_BOTTOM) || ((*config_ptr).mode == F_PWM_OCR_BOTTOM)){
		/*choose overflow mode*/
		TCCR1A = ((*config_ptr).mode & 0x03);
		TCCR1B = (((*config_ptr).mode & 0x0C)<<1);
		/*set LOW for PWM mode*/
		TCCR1A &= ~((1<<FOC1A) | (1<<FOC1B));
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK |= (1<<OCIE1A);
			TIMSK |= (1<<OCIE1B);
		}
	}
	else if(((*config_ptr).mode == CTC_OCR) || ((*config_ptr).mode == CTC_ICR)){
		/*choose overflow mode*/
		TCCR1A = ((*config_ptr).mode & 0x03);
		TCCR1B = (((*config_ptr).mode & 0x0C)<<1);
		/*set HIGH with non-WM mode*/
		TCCR1A |= (1<<FOC1A) | (1<<FOC1B);
		/*Enable Interrupt*/
		if((*config_ptr).interrupt == INT_ENABLE){
			TIMSK |= (1<<OCIE1A);
			TIMSK |= (1<<OCIE1B);
		}
	}
	/*set compare output mode*/
	TCCR1A = (TCCR1A & 0x0F) | ((*config_ptr).output<<4);
	TCCR1A = (TCCR1A & 0x0F) | ((*config_ptr).output<<6);
	/*set pre-scaler*/
	TCCR1B = (TCCR1B & 0xF8) | (*config_ptr).prescaler;
	/*set initial value*/
	TCNT1 = (*config_ptr).initialValue;
	/*set compare value*/
	OCR1A = (*config_ptr).compareValue;
}

/**********************************************************Timer0 Stop Function*******************************************************/
void TIMER1_deinit(void){
	/*Timer stop*/
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	TIMSK &= ~(1<<TOIE1);
}
/*********************************************************ICU Functions****************************************************************/
/*
 * Description: Function to set the required edge detection.
 */
void Icu_setEdgeDetectionType(const edge a_edgeType)
{
	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xBF) | (a_edgeType<<6);
}

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 Icu_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void Icu_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void Icu_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* Disable the Input Capture interrupt */
	TIMSK &= ~(1<<TICIE1);
}

