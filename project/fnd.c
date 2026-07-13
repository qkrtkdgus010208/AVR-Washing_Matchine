/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:36
 *  Author: kccistc
 */ 

#include "fnd.h"

#define ANIM_DISPLAY_UNIT 125
#define TOGGLE_DISPLAY_UNIT 500

static uint32_t ms_count = 0; // ms를 재는 count
static int32_t sec_count = 0; // sec를 재는 count
static uint8_t dot_display = 0;

static uint32_t anim_ms_count = 0;
static uint8_t anim_index = 0;

static uint8_t toggle_display = 0;

static uint32_t stop_watch_ms_count = 0;
static uint32_t stop_watch_sec_count = 0;
static uint8_t run_display = 0;

					        //  0     1     2     3     4     5     6     7     8     9     .
static uint8_t fnd_font[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x80}; // common cathode
	
static uint8_t fnd_circle_anim_right[] = { 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00 }; // common cathode
static uint8_t fnd_circle_anim_left[] = { 0x00, 0x00, 0x00, 0x00, 0x01, 0x20, 0x10, 0x08 }; // common cathode

extern volatile uint32_t msec_count;

extern volatile int fnd_check_time;

extern volatile int forward;

int fnd_main(void)
{
	if (msec_count >= 1)
	{
		msec_count = 0;
		
		ms_count++;
		anim_ms_count++;
		
		if (run_display)
		{
			stop_watch_ms_count++;
			
			if (stop_watch_ms_count >= 1000)
			{
				stop_watch_ms_count = 0;
				stop_watch_sec_count++;
			}
		}
		
		if (anim_ms_count >= ANIM_DISPLAY_UNIT)
		{
			anim_ms_count = 0;
			anim_index = (anim_index + 1) % 8;
		}
		
		if (toggle_display >= TOGGLE_DISPLAY_UNIT)
		{
			toggle_display = !toggle_display;
		}
		
		if (ms_count >= 1000)
		{
			ms_count = 0;
			sec_count++;
			dot_display = !dot_display;
		}
	}
	
	return 0;
}

void init_fnd(void)
{
	FND_DATA_DDR = 0xff; // 출력 모드로 설정
	FND_DIGIT_DDR = 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4; // 자리수 선택
	
	FND_DATA_PORT = 0x00;
}

void fnd_display_min_sec(void)
{
	static int digit_select = 0; // 자리수 선택
	
	FND_DATA_PORT = 0x00;
	
	switch (digit_select)
	{
		case 0: // 1 단위
		FND_DIGIT_PORT = 0x7f;
		FND_DATA_PORT = dot_display ? fnd_font[sec_count % 10] | 0x80 : fnd_font[sec_count % 10];
		break;
		case 1: // 10 단위
		FND_DIGIT_PORT = 0xBF;
		FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
		break;
		case 2: // 100 단위
		FND_DIGIT_PORT = 0xDf;
		FND_DATA_PORT = fnd_font[sec_count / 60 % 10];
		break;
		case 3: // 1000 단위
		FND_DIGIT_PORT = 0xEF;
		FND_DATA_PORT = fnd_font[sec_count / 600 % 6];
		break;
	}
	
	digit_select = (digit_select + 1) % 4; // 다음 표시할 자리수
}

void fnd_display_sec(void)
{
	static int digit_select = 0; // 자리수 선택
	
	FND_DATA_PORT = 0x00;
	
	switch (digit_select)
	{
		case 0: // 1 단위
		FND_DIGIT_PORT = 0x7f;
		FND_DATA_PORT = fnd_font[sec_count % 10]; // 0 ~ 9
		break;
		case 1: // 10 단위
		FND_DIGIT_PORT = 0xBF;
		FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
		break;
		case 2: // 100 단위
		FND_DIGIT_PORT = 0xDf;
		FND_DATA_PORT = fnd_circle_anim_right[anim_index];
		break;
		case 3: // 1000 단위
		FND_DIGIT_PORT = 0xEF;
		FND_DATA_PORT = fnd_circle_anim_left[anim_index];
		break;
	}
	
	digit_select = (digit_select + 1) % 4; // 다음 표시할 자리수
}

void fnd_stop_watch(void)
{
	static int digit_select = 0; // 자리수 선택
	
	FND_DATA_PORT = 0x00;
	
	switch (digit_select)
	{
		case 0: // 1 단위
		FND_DIGIT_PORT = 0x7f;
		FND_DATA_PORT = fnd_font[stop_watch_ms_count % 100 / 10];
		break;
		case 1: // 10 단위
		FND_DIGIT_PORT = 0xBF;
		FND_DATA_PORT = fnd_font[stop_watch_ms_count / 100];
		break;
		case 2: // 100 단위
		FND_DIGIT_PORT = 0xDf;
		FND_DATA_PORT = fnd_font[stop_watch_sec_count % 10];
		break;
		case 3: // 1000 단위
		FND_DIGIT_PORT = 0xEF;
		FND_DATA_PORT = fnd_font[stop_watch_sec_count / 10 % 6];
		break;
	}
	
	digit_select = (digit_select + 1) % 4; // 다음 표시할 자리수
}

void fnd_toggle_stop_watch(void)
{
	run_display = !run_display;
}

void fnd_stop_watch_reset(void)
{
	if (!run_display)
	{
		stop_watch_ms_count = 0;
		stop_watch_sec_count = 0;
	}
}

uint8_t get_stop_watch_run_state(void)
{
	return run_display;
}

void fnd_washing(WASH_STATE wash_state, uint32_t remain_sec)
{
	if (fnd_check_time >= ANIM_DISPLAY_UNIT)
	{
		fnd_check_time = 0;
		
		if (forward || wash_state == SPINNING)
		{
			anim_index = (anim_index + 1) % 8;
		}
		else
		{
			anim_index = (anim_index + 7) % 8;
		}
	}
	
	if (toggle_display >= TOGGLE_DISPLAY_UNIT)
	{
		toggle_display = !toggle_display;
	}
	
	static int digit_select = 0; // 자리수 선택
	static int is_countdown = 0; // 60초 미만 여부

	static int minutes = 0;

	minutes = remain_sec / 60 % 60;
	
	FND_DATA_PORT = 0x00; // 잔상 제거
	
	is_countdown = remain_sec <= 60 ? 1 : 0;
	
	if (is_countdown)
	{
		switch (digit_select)
		{
			case 0: // 1 단위
			FND_DIGIT_PORT = 0x7F;
			FND_DATA_PORT = fnd_font[remain_sec % 10];
			break;
			
			case 1: // 10 단위
			FND_DIGIT_PORT = 0xBF;
			FND_DATA_PORT = fnd_font[remain_sec / 10];
			break;
			
			case 2: // 100 단위
			FND_DIGIT_PORT = wash_state == WAITING || wash_state > SPINNING ? 0xFF : 0xDF;
			FND_DATA_PORT = fnd_circle_anim_right[anim_index];
			break;
			
			case 3: // 1000 단위
			FND_DIGIT_PORT = wash_state == WAITING || wash_state > SPINNING ? 0xFF : 0xEF;
			FND_DATA_PORT = fnd_circle_anim_left[anim_index];
			break;
		}
	}
	else
	{
		switch (digit_select)
		{
			case 0: // 1 단위
			FND_DIGIT_PORT = 0x7F;
			FND_DATA_PORT = fnd_font[minutes % 10];
			break;
			
			case 1: // 10 단위
			FND_DIGIT_PORT = 0xBF;
			FND_DATA_PORT = fnd_font[minutes / 10];
			break;
			
			case 2: // 100 단위
			FND_DIGIT_PORT = wash_state == WAITING || wash_state > SPINNING ? 0xFF : 0xDF;
			FND_DATA_PORT = fnd_circle_anim_right[anim_index];
			break;
			
			case 3: // 1000 단위
			FND_DIGIT_PORT = wash_state == WAITING || wash_state > SPINNING ? 0xFF : 0xEF;
			FND_DATA_PORT = fnd_circle_anim_left[anim_index];
			break;
		}
	}
	
	digit_select = (digit_select + 1) % 4;
}