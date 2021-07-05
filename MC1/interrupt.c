/*
 * interrupt.c
 *
 *  Created on: 7 Jun 2021
 *      Author: A.Rifaat
 */

#include "interrupt.h"

void EnableGlobalInterrupt(void){
	SET_BIT(SREG,7);
}

void DisableGlobalInterrupt(void){
	CLEAR_BIT(SREG,7);
}
