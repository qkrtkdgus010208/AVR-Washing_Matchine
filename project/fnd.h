/*
 * fnd.h
 *
 * Created: 2026-06-12 오전 10:46:18
 *  Author: kccistc
 */ 

#ifndef FND_H_
#define FND_H_

#define F_CPU 16000000UL
#include "washing.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // sei, cli 등 함수

#define FND_DATA_DDR   DDRC
#define FND_DATA_PORT  PORTC

#define FND_DIGIT_DDR  DDRB
#define FND_DIGIT_PORT PORTB
#define FND_DIGIT_D1   4
#define FND_DIGIT_D2   5
#define FND_DIGIT_D3   6
#define FND_DIGIT_D4   7

int fnd_main(void);
void init_fnd(void);
void fnd_display_min_sec(void);
void fnd_stop_watch(void);
void fnd_toggle_stop_watch(void);
uint8_t get_stop_watch_run_state(void);
void fnd_washing(WASH_STATE wash_state, uint32_t remain_sec);

#endif /* FND_H_ */