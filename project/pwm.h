/*
 * pwm.h
 *
 * Created: 2026-06-18 오후 2:23:17
 *  Author: kccistc
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "common_types.h"
#include "button.h"
#include <avr/interrupt.h>
#include <stdio.h>

void init_timer3_pwm(void);
void init_motor_driver(void);
void dcmotor_control_main(void);
void dcmotor_washing(WASH_STATE wash_state);

#endif /* PWM_H_ */