/*
 * washing.c
 *
 * Created: 2026-06-18 오후 4:14:45
 *  Author: kccistc
 */ 

#include "washing.h"

static WASH_STATE current_state = WAITING;
static WASH_STATE prev_state = WASHING;

static uint32_t washing_sec = 10; // 기본값 10초
static uint32_t rinsing_sec = 10;
static uint32_t spinning_sec = 10;
static uint32_t remain_sec = 0;

static uint32_t is_start = 1;

extern volatile uint32_t sec_count;

void init_washing_matchine(void)
{
	current_state = WAITING;
	prev_state = WASHING;
	
	washing_sec = 10;
	rinsing_sec = 10;
	spinning_sec = 10;
	remain_sec = 0;
	
	is_start = 1;
}

void washing_matchine_main(void)
{		
	while (1)
	{
		time_check();
		input_check();
		washing_machine_processing();
	}
}

void time_check(void)
{
	if (sec_count >= 1)
	{
		sec_count = 0;
		printf("%d\n", remain_sec);
	
		if (current_state != WAITING && current_state < SET_WASH_TIME) 
		{
			printf("%d\n", remain_sec);
	
			remain_sec--;
			
			if (remain_sec <= 0) time_over_next_state();
		}
	}
}

void input_check(void)
{
	if (get_button(BUTTON0, BUTTON0PIN)) // 시작 / 정지
	{
		if (current_state == WAITING) // 대기 상태
		{
			current_state = prev_state;
			
			if (is_start)
			{
				is_start = 0;
				remain_sec = washing_sec;
			}
		}
		else
		{
			prev_state = current_state; // 이전 상태 저장
			current_state = WAITING;
		}
	}
	else if (get_button(BUTTON1, BUTTON1PIN)) // 모드 변경
	{
		if (current_state == WAITING || current_state >= SET_WASH_TIME)
		{
			static int prev_remain_sec = 0;
			
			if (current_state == WAITING)
			{
				prev_remain_sec = remain_sec; // 기존 잔여 시간 저장
			
				current_state = SET_WASH_TIME; // 대기모드 -> 세탁 시간 설정 진입
				remain_sec = washing_sec;       // 현재 설정된 시간 FND에 보여주기
			}
			else if (current_state == SET_WASH_TIME)
			{
				current_state = SET_RINSE_TIME; // 세탁 설정 -> 헹굼 설정
				remain_sec = rinsing_sec;
			}
			else if (current_state == SET_RINSE_TIME)
			{
				current_state = SET_SPIN_TIME;  // 헹굼 설정 -> 탈수 설정
				remain_sec = spinning_sec;
			}
			else if (current_state == SET_SPIN_TIME)
			{
				current_state = WAITING;        // 탈수 설정 -> 다시 대기모드 복귀
				remain_sec = prev_remain_sec; // 기존 잔여시간 FND에 보여주기
			}
		}
	}
	else if (get_button(BUTTON2, BUTTON2PIN)) // 시간 설정
	{
		if (current_state == SET_WASH_TIME)
		{
			washing_sec += 60; // 60초씩 증가
			remain_sec = washing_sec; // 증가된 시간 즉시 반영
		}
		else if (current_state == SET_RINSE_TIME)
		{
			rinsing_sec += 60;
			remain_sec = rinsing_sec;
		}
		else if (current_state == SET_SPIN_TIME)
		{
			spinning_sec += 60;
			remain_sec = spinning_sec;
		}
	}
	else if (get_button(BUTTON3, BUTTON3PIN)) // 테스트
	{
		if (current_state < SET_WASH_TIME) 
			time_over_next_state();
	}
}

void washing_machine_processing(void)
{
	dcmotor_washing(current_state);
	led_washing(current_state);
	fnd_washing(current_state, remain_sec);
}

void time_over_next_state(void)
{
	current_state = (current_state + 1) % 4;
	prev_state = current_state;
	
	switch (current_state)
	{
		case WAITING:
		init_washing_matchine();
		break;
		
		case WASHING:
		remain_sec = washing_sec;
		break;
		
		case RINSING:
		remain_sec = rinsing_sec;
		break;
		
		case SPINNING:
		remain_sec = spinning_sec;
		break;
	}
}