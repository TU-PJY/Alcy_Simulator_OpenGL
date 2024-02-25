#include "globalVar.h"
#include "screen.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "ZZZ.h"
#include "Icon.h"
#include "Background.h"
#include "White.h"
#include "Turntable.h"
#include "Speaker.h"
#include "Guitar.h"
#include "Arm.h"
#include "Light.h"
// 프로젝트 전체 전역 변수 및 클래스

GLfloat mx, my; // 마우스 좌표
GLfloat ratio_;  // 화면 종횡비;

bool lButtonDown;  // 마우스 클릭 여부
bool mouseClickEnable;  // 마우스 클릭 사용 가능 여부
bool startIntro;  // 인트로 재생, Camera 객체 전용
bool gameStarted; // 게임 시작을 알리는 변수

bool playFunc;  // 음악 재생 여부
int funcNumber;  // 음악 구분
double beatDelay;  // 음악 효과에 사용되는 딜레이 변수
GLfloat beatVal;  // 음악 효과 수치
GLfloat interpolation;
GLfloat interval, playTime;
GLfloat beatAcc;
GLfloat functionOperationTime;

GLfloat whiteTransparent;  // 흰 배경 투명도

// 여기에 클래스 선언하고 사용
Background background;
Camera cam;
Alcy alcy;
ZZZ zzz[3];
UI ui;
Icon icon[ICON_PART];
White white;
Turntable turntable;
Speaker speaker;
Guitar guitar;
Arm arm;
Light light;