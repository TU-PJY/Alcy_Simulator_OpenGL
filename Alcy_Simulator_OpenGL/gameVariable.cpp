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

// 알키 관련 변수
int dir = m;  // 알키 바라보는 방향, 초기값 m
bool blinkEnable = false; // 눈 깜빡임 여부, true일 시 깜빡임 활성화
time_t startTime = time(NULL), endTime;  // 눈 깜빡임 간격 타이머
GLfloat blinkInterval = 3;  // 눈 깜빡임 간격, 기본 3초
int keepTimer;  // 눈을 감은 상태를 아주 짧게 유지한다

// 이미지 투명도
GLfloat transparent;

//알키 머리, 얼굴 파츠 움직임
GLfloat headPos;  // 방향 전환 시 전환한 방향으로 움직임