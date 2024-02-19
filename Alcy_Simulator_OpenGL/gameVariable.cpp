#include "gl_func.h"
// 여기에 게임 변수 선언

// 마우스
GLfloat mx, my; 

// 일시정지
bool pause = false;
GLfloat pauseAcc;  // 일시정지 애니메이션 가속

// 카메라
GLfloat ratio;  // 화면 종횡비;
GLfloat camX, camY;  // 게임 화면 카메라
GLfloat camRot;  // 카메라 회전 각도
GLfloat zoom = 1.0;  // 카메라 줌
GLfloat zoomAcc = 0;  // 줌 가속도
bool zoomEnable = false;  // 줌 여부
bool camR, camL;  // 카메라 좌우 회전

// 커서
bool cursorEnable = true;  // 커서 보임 여부
bool handEnable = false;  // 손커서 전환 여부

// 알키 관련 변수
int dir = m;  // 알키 바라보는 방향, 초기값 m
bool blinkEnable = false; // 눈 깜빡임 여부, true일 시 깜빡임 활성화
time_t startTime = time(NULL), endTime;  // 눈 깜빡임 간격 타이머
GLfloat blinkInterval = 3;  // 눈 깜빡임 간격, 기본 3초
GLfloat keepTimer;  // 눈을 감은 상태를 아주 짧게 유지한다
GLfloat alcyHeight;  // 일시정지 시 알키이미지가 위로 조금 올라간다

// 이미지 투명도
GLfloat transparent;

//알키 머리, 얼굴 파츠 움직임
GLfloat headPos;  // 방향 전환 시 전환한 방향으로 움직임