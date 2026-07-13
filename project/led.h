/*
 * led.h
 *
 * Created: 2026-06-10 오후 3:09:22
 *  Author: user
 */ 


#ifndef LED_H_
#define LED_H_

#define F_CPU 16000000UL  // 16MHz
#include "common_types.h"
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등

void init_led(void);
int led_main(void);
void led_all_on(void);
void led_all_off(void);
void led_shift_left_on(void);
void led_shift_right_on(void);
void led_shift_left_keep_on(void);
void led_shift_right_keep_on(void);
void led_flower_on(void);
void led_flower_off(void);
void trigger_led_control(int);

#endif /* LED_H_ */