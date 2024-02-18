#include "gl_func.h"
// 여기에 게임 변수 선언

// 마우스
GLfloat mx, my; 

// 카메라
GLfloat ratio;  // 화면 종횡비;
GLfloat camX, camY;  // 게임 화면 카메라
GLfloat camRot;  // 카메라 회전 각도
bool camR, camL;  // 카메라 좌우 회전

// 커서
bool cursorEnable = true;

// 알키 바라보는 방향
int dir = m;  // 초기값 m

// 이미지 투명도
GLfloat transparent;

//알키 머리, 얼굴 파츠 움직임
GLfloat headPos;  // 방향 전환 시 전환한 방향으로 움직임