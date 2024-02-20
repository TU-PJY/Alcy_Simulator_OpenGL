#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include "config.h"
// 여기에 프로젝트 전역 변수 선언

// 마우스
extern GLfloat mx, my;  // 마우스 좌표
extern bool lButtonDown;  // 마우스 클릭 여부
extern bool mouseClickEnable;  // 마우스 클릭 가능 여부

// 프레임
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, 프레임 동기화

#endif