/*
 * washing.h
 *
 * Created: 2026-06-18 오후 4:14:37
 *  Author: kccistc
 */ 


#ifndef WASHING_H_
#define WASHING_H_

#include "common_types.h"
#include "button.h"
#include "pwm.h"
#include <avr/interrupt.h>
#include <stdio.h>

void init_washing_matchine(void);
void washing_matchine_main(void);
void time_check(void);
void input_check(void);
void washing_machine_processing(void);
void time_over_next_state(void);

#endif /* WASHING_H_ */