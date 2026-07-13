/*
 * common_types.h
 *
 * Created: 2026-06-18 오후 4:57:49
 *  Author: kccistc
 */ 


#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

typedef enum {
	WAITING, // 대기 상태 (기본값 0)
	WASHING, // 세탁 상태 (1)
	RINSING, // 헹굼 상태 (2)
	SPINNING, // 탈수 상태 (3)
	
	SET_WASH_TIME, // 세탁 시간 설정 모드
	SET_RINSE_TIME, // 헹굼 시간 설정 모드
	SET_SPIN_TIME // 탈수 시간 설정 모드
} WASH_STATE;

#endif /* COMMON_TYPES_H_ */