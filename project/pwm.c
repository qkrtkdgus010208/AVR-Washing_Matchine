/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:02
 *  Author: kccistc
 */ 

#include "pwm.h"

extern volatile int pwm_check_time;

volatile int forward = 1;

// 16bit 3번 timer/counter를 사용
/*
	PWM 출력 신호
	============
	PE3: OC3A
	PE4: OC3B: INT4 사용(초음파 센서)
	PE5: OC3C ==> DC MOTOR (PWM 출력)
	BTN1: speed-up
	BTN2: speed-down
*/
void init_timer3_pwm(void)
{
	// PE4 초음파 연결
	DDRE |= 1 << 3 | 1 << 5;
	
	// 모드 5: 8bit 고속 PWM 모드를 사용하겠다. timer3, 348p 표 15-6
	TCCR3A |= 1 << WGM30;
	TCCR3B |= 1 << WGM32;
	
	// 비반전 모드 top: 0x00ff 비교일치 값 (PWM) 지정 OCR3C, 350p 표 15-7
	TCCR3A |= 1 << COM3C1;
	
	// ---- 분주비 설정 ----
	// 분주비 64
	// 16000000Hz / 64 --> 250000HZ (250KHz)
	// T = 1 / f -> 1 / 250000Hz --> 0.000004sec ---> 4us
	// 250000Hz에서 256개의 펄스를 count하면 소요시간: 1.024ms
	// 250000Hz에서 128개의 펄스를 count하면 소요시간: 0.512ms
	// TCNT3: 0 ~ 255(0x00ff)까지 count한 후 0으로 다시 돌아간다.
	TCCR3B |= 1 << CS31 | 1 << CS30; // 분주비 64
	OCR3C = 0; // OCR (Output Compare Register): PWM 값
	// OCR3C가 50인 경우 Duty(High)가 몇 %인가?
	// Duty Cycle: (OCR3C / Top) x 100 = 50 / 255 x 100 = 19.61%
}

/*
	PE5: PWM control
	PF6: IN1 방향 설정
	PF7: IN2
*/
void init_motor_driver(void)
{
	DDRF |= 1 << 6 | 1 << 7; // 출력 모드로 설정
	PORTF &= ~(1 << 6 | 1 << 7); // 6, 7 reset
	PORTF |= 1 << 6; // 정회전
}

void dcmotor_control_main(void)
{
	int start_button = 0;
	int forward = 1;
	
	while (1)
	{
		if (get_button(BUTTON0, BUTTON0PIN)) // start stop
		{
			start_button = !start_button;
			if (start_button) OCR3C = 250;
			else OCR3C = 0;
		}
		else if (get_button(BUTTON1, BUTTON1PIN)) // speed up
		{
			if (OCR3C >= 250) OCR3C = 250;
			else OCR3C += 20;
		}
		else if (get_button(BUTTON2, BUTTON2PIN)) // speed down
		{
			if (OCR3C <= 70) OCR3C = 70;
			else OCR3C -= 20;
		}
		else if (get_button(BUTTON3, BUTTON3PIN)) // 방향 설정
		{
			forward = !forward;
			PORTF &= ~(1 << 6 | 1 << 7); // 6, 7 reset
			if (forward) PORTF |= (1 << 6);
			else         PORTF |= (1 << 7);
		}
	}
}

void dcmotor_washing(WASH_STATE wash_state)
{
	
	if (pwm_check_time >= 2000) // 2초마다 방향 변경
	{
		pwm_check_time = 0;
		forward = !forward;
	}
	
	switch (wash_state)
	{
		case WAITING:
		OCR3C = 0;
		PORTF &= ~(1 << 6 | 1 << 7); // 6, 7 reset
		break;
		
		case WASHING:
		OCR3C = 250;
		PORTF &= ~(1 << 6 | 1 << 7);
		if (forward) PORTF |= (1 << 6); // 정회전
		else         PORTF |= (1 << 7); // 역회전
		break;
		
		case RINSING:
		OCR3C = 200;
		PORTF &= ~(1 << 6 | 1 << 7);
		if (!forward) PORTF |= (1 << 6);
		else         PORTF |= (1 << 7);
		break;
		
		case SPINNING:
		OCR3C = 150;
		PORTF |= (1 << 6);
		break;
	}
}