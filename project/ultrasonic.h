/*
 * ultrasonic.h
 *
 * Created: 2026-06-17 오후 1:32:35
 *  Author: kccistc
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // sei, cli 등 함수

#define TRIG_DDR	DDRG
#define TRIG_PORT	PORTG
#define TRIG_PIN	4

#define ECHO_DDR	DDRE
#define ECHO_PORT	PINE // external INT 4
#define ECHO_PIN	4

void init_ultrasonic(void);
void make_trigger(void);
void ultrasonic_processing(void);

#endif /* ULTRASONIC_H_ */