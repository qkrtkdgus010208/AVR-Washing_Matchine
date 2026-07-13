/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 

#include "led.h"

#define DELAY_TIME 100
#define FUNC_COUNT 6
 
extern volatile uint32_t msec_count; // volatile로 최적화 방지

int func_state = 0;

void (*fp[]) () =
{
	led_shift_left_on, // func_state = 0
	led_shift_right_on,
	led_shift_left_keep_on,
	led_shift_right_keep_on,
	led_flower_on,
	led_flower_off // func_state = 5
};

int led_main(void)
{
	led_all_off();
	
	while (1)
	{
		fp[func_state]();
	}
	
	return 0;
}

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off 	
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
}

void led_shift_left_on(void)
{
	static int i = 0;
	
	if (msec_count >= DELAY_TIME)
	{
		msec_count = 0;
		
		PORTA = 1 << i; // PORTA 주소: 0x3B
		
		i = (i + 1) % 8;
		//if ((i = (i + 1) % 8) == 0) // 다음 index 값을 계산
			//func_state = (func_state + 1) % FUNC_COUNT; // 다음 실행할 func으로 jump
	}
}

void led_shift_right_on(void)
{
	static int i = 0;
	
	if (msec_count >= DELAY_TIME)
	{
		msec_count = 0;
		
		PORTA = 0x80 >> i; // PORTA 주소: 0x3B
		
		i = (i + 1) % 8;
		//if ((i = (i + 1) % 8) == 0) // 다음 index 값을 계산
			//func_state = (func_state + 1) % FUNC_COUNT; // 다음 실행할 func으로 jump
	}
}

void led_shift_left_keep_on(void)
{
	static int i = 0;
	
	if (msec_count >= DELAY_TIME)
	{
		msec_count = 0;
		
		PORTA |= 1 << i; // PORTA 주소: 0x3B
		
		i = (i + 1) % 8;
		//if ((i = (i + 1) % 8) == 0) // 다음 index 값을 계산
			//func_state = (func_state + 1) % FUNC_COUNT; // 다음 실행할 func으로 jump
	}
}

void led_shift_right_keep_on(void)
{
	static int i = 0;
	
	if (msec_count >= DELAY_TIME)
	{
		if (i == 0) led_all_off();
	
		msec_count = 0;
		
		PORTA |= 0x80 >> i; // PORTA 주소: 0x3B
		
		i = (i + 1) % 8;
		//if ((i = (i + 1) % 8) == 0) // 다음 index 값을 계산
			//func_state = (func_state + 1) % FUNC_COUNT; // 다음 실행할 func으로 jump
	}
}

void led_flower_on(void)
{
	static int i = 0;
	
	if (msec_count >= DELAY_TIME * 2)
	{
		if (i == 0) led_all_off();
	
		msec_count = 0;
		
		PORTA |= 1 << (4 - i) | 0x80 >> (4 - i); // PORTA 주소: 0x3B
		
		i = (i + 1) % 4;
		//if ((i = (i + 1) % 4) == 0) // 다음 index 값을 계산
			//func_state = (func_state + 1) % FUNC_COUNT; // 다음 실행할 func으로 jump
	}
}

void led_flower_off(void)
{
	static int i = 0;
	
	if (msec_count >= DELAY_TIME * 2)
	{
		msec_count = 0;
		
		PORTA &= ~(1 << i | 0x80 >> i); // PORTA 주소: 0x3B
		
		i = (i + 1) % 4;
		//if ((i = (i + 1) % 4) == 0) // 다음 index 값을 계산
			//func_state = (func_state + 1) % FUNC_COUNT; // 다음 실행할 func으로 jump
	}
}

void trigger_led_control(int distance_cm)
{
	// 2cm 이하: 0번 LED 켜기 (0b00000001)
	if (distance_cm <= 2)
	{
		PORTA = 0x01;
	}
	// 3cm 이하: 0~1번 LED 켜기 (0b00000011)
	else if (distance_cm <= 3)
	{
		PORTA = 0x03;
	}
	// 4cm 이하: 0~2번 LED 켜기 (0b00000111)
	else if (distance_cm <= 4)
	{
		PORTA = 0x07;
	}
	// 5cm 이하: 0~3번 LED 켜기 (0b00001111)
	else if (distance_cm <= 5)
	{
		PORTA = 0x0F;
	}
	// 6cm 이하: 0~4번 LED 켜기 (0b00011111)
	else if (distance_cm <= 6)
	{
		PORTA = 0x1F;
	}
	// 7cm 이하: 0~5번 LED 켜기 (0b00111111)
	else if (distance_cm <= 7)
	{
		PORTA = 0x3F;
	}
	// 10cm 이하: 0~6번 LED 켜기 (0b01111111)
	else if (distance_cm <= 10)
	{
		PORTA = 0x7F;
	}
	// 10cm 초과: 모든 LED 켜기 (0b11111111)
	else
	{
		PORTA = 0xFF;
	}
}

void led_washing(WASH_STATE wash_state)
{
	switch (wash_state)
	{
		case WAITING: // 대기 중이면 off
		PORTA = 0x00;
		break;
		
		case WASHING: // 세탁 중이면 1번 on
		PORTA = 0x01;
		break;
		
		case RINSING: // 헹굼 중이면 2번 on
		PORTA = 0x02;
		break;
		
		case SPINNING: // 탈수 중이면 3번 on
		PORTA = 0x04;
		break;
	}
}