#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H
#include "config.h"
// 여기에 게임 변수 선언

// 마우스
extern GLfloat mx, my;

extern GLfloat ratio;

// 프레임
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, 프레임 동기화

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