#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H
#include "config.h"
// 여기에 게임 변수 선언

// 마우스
extern GLfloat mx, my;

// 프레임
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, 프레임 동기화

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

#endif