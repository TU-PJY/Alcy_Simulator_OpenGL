#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include "config.h"
// 여기에 프로젝트 전역 변수 선언

// 마우스
extern GLfloat mx, my;  // 마우스 좌표
extern bool lButtonDown;  // 마우스 클릭 여부
extern bool mouseClickEnable;  // 마우스 클릭 가능 여부
extern bool startIntro;
extern bool gameStarted;

// 프레임
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, 프레임 동기화

// 음악 재생, true일 시 해당하는 음악이 재생되고, 박자에 맞추어 알키가 움직이고 카메라가 움직인다.
extern bool playMusic;  
extern int musicTrack;
extern double beatDelay;  // 음악 효과에 사용되는 딜레이 변수

#endif