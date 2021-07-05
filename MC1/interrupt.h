/*
 * interrupt.h
 *
 *  Created on: 7 Jun 2021
 *      Author: A.Rifaat
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include"common_macros.h"
#include"micro_config.h"
#include"std_types.h"

void EnableGlobalInterrupt(void);
void DisableGlobalInterrupt(void);

#endif /* INTERRUPT_H_ */
