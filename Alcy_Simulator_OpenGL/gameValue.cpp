#include "gl_func.h"
// 여기에 게임 변수 선언
GLfloat mx, my; // 마우스 위치

// 카메라
GLfloat camX, camY;  // 게임 화면 카메라
GLfloat camRot;  // 카메라 회전 각도
bool camR, camL;  // 카메라 좌우 회전

// 커서
bool cursorEnable = true;