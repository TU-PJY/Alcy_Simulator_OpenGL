#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include "config.h"
// 여기에 프로젝트 전역 변수 선언

// 마우스
extern GLfloat mx, my;  // 마우스 좌표
extern bool lButtonDown;  // 마우스 클릭 여부
extern bool mouseClickEnable;  // 마우스 클릭 가능 여부
extern bool startIntro;  // 카메라 인트로 애니메이션 활성화 여부
extern bool gameStarted;  // 게임 시작을 알리는 변수

// 프레임
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, 프레임 동기화

// 메뉴바 기능
extern bool playFunc; // 기능 실행 여부
extern int funcNumber;  // 기능 번호
extern double beatDelay;  // 박자 효과에 사용되는 딜레이 변수
extern GLfloat interpolation;  // 박자 효과 지연을 보간하는 변수 
extern GLfloat interval, playTime;  // 지정 박자 간격, 지정 기능 실핼 시간
extern GLfloat beatAcc;  // 박자 효과 가속값
extern GLfloat beatVal;  // 박자 효과 수치
extern GLfloat functionOperationTime;  // 기능 실행 시간

// 종료 / 정보창
extern int ExitOrInfo;  // 종료 또는 정보창 구분 숫자, 0: 종료, 1: 정보창
extern bool setInfo;  // true일 시 정보 표시 활성화

// UI
extern GLfloat whiteTransparent;  // 흰 배경 투명도

// 로딩
extern bool LoadingEnd;   // 리소스 로딩 완료 여부

#endif