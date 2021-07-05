/*
 * motors.h
 *
 *  Created on: 10 Jun 2021
 *      Author: A.Rifaat
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

void MOTOR_CW(void); /*Start Motor Clock wise*/
void MOTOR_ACW(void); /*Start Motor Anti Clock wise*/
void MOTOR_stop(void); /*Stop Motor*/

#endif /* MOTORS_H_ */
