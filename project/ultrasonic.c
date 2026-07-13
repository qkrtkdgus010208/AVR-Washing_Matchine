/*
 * ultrasonic.c
 *
 * Created: 2026-06-17 오후 1:33:09
 *  Author: kccistc
 */ 

#include "ultrasonic.h"

volatile int ultrasonic_distance = 0;
volatile char scm[50];
volatile int distance_cm = 0;
volatile int falling_edge_flag = 0;

extern volatile int ultrasonic_check_time;

// 278p 표 12-3
// INT4: PE4 외부 INT4 초음파 센서 상승, 하강 에지 발생 시 이곳으로 진입
// 결국 상승 에지 때 1번 들어오고 하강 에지 때 1번 들어온다.
// 0x000A
ISR(INT4_vect)
{
	// 1. 상승 에지
	if (ECHO_PORT & (1 << ECHO_PIN))
	{
		TCNT1 = 0;
	}
	else // 2. 하강 에지
	{
		// TCNT1: Timer count 1
		// 예) TCNT1이 10이 들어있다고 가정
		// 15.625KHz의 1주기: 64us
		// 0.000064sec * 10개 ==> 0.00064sec(640us)
		// 640us / 58(1cm 이동하는데 소요시간): 11cm
		// 1sec: 1000000us
		
		ultrasonic_distance = TCNT1 * 1000000.0 * 1024 / F_CPU;
		
		//--- 소요 시간을 cm로 환산
		distance_cm = ultrasonic_distance / 58;
		sprintf(scm, "dis: %dcm\n", distance_cm); // cm로 환산
		
		falling_edge_flag = 1;
	}
}

void init_ultrasonic(void)
{
	TRIG_DDR |= 1 << TRIG_PIN; // output mode로 설정
	ECHO_DDR &= ~(1 << ECHO_PIN); // input mode로 설정
	
	// 288p 그림 12-8, 289p 표 12-6 참조
	// 0 1: 상승 에지, 하강 에지에서 둘 다 INT를 띄우도록 요청한다.
	EICRB |= 0 << ISC41 | 1 << ISC40;
	
	// 16bit timer/counter 1번을 사용하기로 하자
	// timer INT를 사용하지 않는다.
	// 16bit timer1 16bit로 표시할 수 있는 최대값 65535(max): 0xffff
	// 16Mz / 1024 분주: 16000000Hz / 1024 --> 15625Hz --> 15.625KHz
	// 1주기(1개의 필수 소요시간): T = 1 / f --> 1 / 15625 --> 0.000064sec => 64us
	// 분주비 1024 설정, 318p 표 14-1
	TCCR1B |= 1 << CS12 | 1 << CS10; // 1024 분주
	
	//---- EINT4 설정 ----
	// 287p 그림 12-6
	EIMSK |= 1 << INT4; // 외부 INT4(ECHO핀)
}

void make_trigger(void)
{
	TRIG_PORT &= ~(1 << TRIG_PIN); // low로 만든다
	_delay_us(1);
	TRIG_PORT |= 1 << TRIG_PIN; // high로 만든다
	_delay_us(15); // 규격에는 10us인데 reduancy로 15us
	TRIG_PORT &= ~(1 << TRIG_PIN); // low로 만든다
}

void ultrasonic_processing(void)
{
	if (falling_edge_flag == 1)
	{
		falling_edge_flag = 0;
		printf("%s", scm);
		trigger_led_control(distance_cm);
		_delay_ms(50);
		make_trigger();
	}
}