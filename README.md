# 📝 AVR 세탁기 프로젝트

---

# 📚 목차

- 프로젝트 개요
- BOM Table
- 외관 사진
- FSM
- 세부 기능
- 작동 영상
- 프로젝트 발전 방향

---

# 프로젝트 개요

### 프로젝트 목적
- AVR의 ATmega128A 마이크로컨트롤러에 대한 이해
- LED, BUTTON, FND, PWM, UART, BT, ULTRASONIC 등을 프로젝트에 활용

### 프로젝트 목표
- 세탁기의 기본 동작(세탁, 헹굼, 탈수) 구현
- 각 동작 별 시간 설정 기능 구현

---

# BOM Table

<img width="838" height="373" alt="image" src="https://github.com/user-attachments/assets/bda42c11-214a-4b29-9da3-4d8771736a0e" />

---

# 외관 사진

<img width="887" height="1121" alt="image" src="https://github.com/user-attachments/assets/7f13ae55-ac9a-4999-9349-92171e25400b" />

---

# FSM

<img width="1814" height="1080" alt="image" src="https://github.com/user-attachments/assets/73a804fa-e31e-4a58-864d-a38ee0cd085a" />

---

# 세부 기능

<img width="792" height="285" alt="image" src="https://github.com/user-attachments/assets/8e2101e8-d278-47ad-b328-aa8a9a9c50e4" />

---

# 작동 영상

|세탁/헹굼/탈수|시간 설정|
|:---:|:---:|
| [![스마트 세탁기 작동 시연 (세탁/헹굼/탈수)](https://img.youtube.com/vi/z_TE3xN_G6E/hqdefault.jpg)](https://youtube.com/shorts/z_TE3xN_G6E) | [![스마트 세탁기 작동 시연 (시간 설정)](https://img.youtube.com/vi/z_TE3xN_G6E/hqdefault.jpg)](https://youtube.com/shorts/z_TE3xN_G6E) |

---

# 프로젝트 발전 방향

### 상태패턴 도입
- 상태 패턴을 도입해 SWITCH-CASE 문으로 구현된 FSM을 각 상태별 함수 포인터나 구조체 배열 형태로 분리
### 비휘발성 설정 메모리 백업
- 사용자가 버튼 제어로 커스텀 설정한 모드별 세탁 시간 데이터를 ATMEGA128 내부 EEPROM 영역에 저장하여, 전원이 완전히 차단되었다가 다시 켜져도 이전 세탁 설정을 그대로 기억하고 복원하는 편의 기능 제공
