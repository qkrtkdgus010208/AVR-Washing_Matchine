/*
 * 07.MY_WASHING_MATCHINE.c
 *
 * Created: 2026-06-18 오후 4:12:56
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // sei, cli 등 함수
#include <stdio.h>
#include "button.h"
#include "fnd.h"
#include "led.h"
#include "uart0.h"
#include "ultrasonic.h"
#include "pwm.h"
#include "washing.h"

volatile uint32_t msec_count = 0; // volatile로 최적화 방지
volatile uint32_t sec_count = 0;
volatile int ultrasonic_check_time = 0;
volatile int pwm_check_time = 0;
volatile int fnd_check_time = 0;

static void init_timer0(void);

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

extern volatile int ultrasonic_distance;
extern volatile int falling_edge;

/*
ISR(Interrupt Service Routine): 인터럽트 처리 함수 ISR로 시작
TIMER0_OVF_vect: Timer 0 Overflow INT가 발생이 되면 이곳으로 진입함
250개의 펄스를 count(1ms)하면 이곳으로 자동 진입한다.
ISR은 가능한 짧게 작성한다.
*/
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // TCNT0 6 ~ 256 : 250개 펄스 count하기 위해
	msec_count++; // 1ms count
	
	if (msec_count >= 1000)
	{
		msec_count = 0;
		sec_count++; // 1sec count
	}
	
	ultrasonic_check_time++;
	pwm_check_time++;
	fnd_check_time++;
}

int main(void)
{
	// 초기화 로직
	init_button();
	init_fnd();
	init_led();
	init_timer0();
	init_uart0();
	init_timer3_pwm();
	init_motor_driver();
	init_washing_matchine();
	
	stdout = &OUTPUT;
	sei();
		
    washing_matchine_main(); // 세탁기 기능 구현 함수
}

/*
1. timer0을 초기화한다.
   AVR에서 8bit timer 0/2 중에서 0번을 초기화한다.
   임베디드에서 가장 신경을 써야 할 부분이 초기화하는 것이다.
   초기화가 잘못되면 이후가 다 꼬인다.
   
2. 8bit가지고 1ms를 측정하는 timer/counter를 만들고자 한다.
 2.1. 분주비 설정
      16,000,000Hz / 64 ==> 250,000Hz
	  16,000,000Hz / 256 ==> 62,500Hz
	  16,000,000Hz / 1024 ==> 15,625Hz
 2.2. 1주기가 잡아먹는 시간 계산
      T = 1/f = 1/250,000 = 0.000004s = 0.004ms = 4us
	  T = 1/f = 1/62,500 = 0.000016s = 0.016ms = 16us
	  T = 1/f = 1/15,625 = 0.000064s = 0.064ms = 64us
 2.3. 8bit가지고 count하는 시간을 계산 (8bit timer OV(OV flow))
      0.004ms * 256개 ==> 1.024ms => 0.001024s
	  0.004ms * 250개 ==> 1ms -> 0.001s
	  0.016ms * 256개 ==> 4.096ms => 0.004096s
	  0.016ms * 250개 ==> 4ms -> 0.004s
	  0.064ms * 64개 ==> 4.096ms => 0.004096s
*/

void init_timer0(void)
{
	TCNT0 = 6; // TCNT0 6 ~ 256 : 250개 펄스 count하기 위해
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 0분주
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; // 64분주
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT
}

