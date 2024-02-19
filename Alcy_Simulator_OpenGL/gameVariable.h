#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H
#include "config.h"
// 여기에 게임 변수 선언

// 마우스
extern GLfloat mx, my;

// 카메라
extern GLfloat ratio;  // 화면 종횡비;
extern GLfloat camX, camY;  // 게임 화면 카메라
extern GLfloat camRot;  // 카메라 회전 각도
extern GLfloat zoom;  // 카메라 줌
extern GLfloat zoomAcc;  // 줌 가속도
extern bool zoomEnable;  // 줌 여부
extern bool camR, camL;  // 카메라 좌우 회전

// 커서
extern bool cursorEnable;  // 커서 보임 여부
extern bool handEnable;  // 손커서 전환 여부
extern GLfloat handX;  // 손커서 x좌표, 쓰다듬기 시 사용
extern GLfloat handNum;  // 손 커서 애니메이션에 사용되는 수치

// 나가기 아이콘
extern bool exitEnable;
extern GLfloat exitTransparent; // 나가기 아이콘 투명도

//팁
extern bool tipEnable;
extern GLfloat tipTransparent;

//// 알키 관련 변수 ////
extern int dir;  // 알키 바라보는 방향, 초기값 m
extern bool blinkEnable; // 눈 깜빡임 여부, true일 시 깜빡임 활성화
extern time_t startTime, endTime, blinkTime;  // 눈 깜빡임 간격 타이머
extern GLfloat blinkInterval;  // 눈 깜빡임 간격, 기본 3초
extern GLfloat keepTimer;  // 눈을 감은 상태를 아주 짧게 유지한다
extern bool touchEnable;  // 알키 머리 쓰다듬기 여부

//알키 머리, 얼굴 파츠 움직임
extern GLfloat headPos;  // 방향 전환 시 전환한 방향으로 움직임
extern GLfloat headRot;  // 쓰다듬기 시 머리 회전 각도
extern GLfloat tailRot;  // 쓰다듬기 시 꼬리 회전 각도
extern GLfloat bodyRot;  // 쓰다듬기 시 몸통 회전 각도
extern GLfloat tailNum; // 꼬리 회전에 사용되는 수치
extern bool headTiltR, headTiltL;  // 카메라 기울였을 때 알키 머리 기울이기 여부

#endif