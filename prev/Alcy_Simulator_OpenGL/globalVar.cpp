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
#include "Mic.h"
#include "Neon.h"
#include "Note.h"
#include "Info.h"
#include "Start.h"
// 프로젝트 전체 전역 변수 및 클래스

// 프레임
int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

// 화면
GLfloat ratio_;  // 화면 종횡비

// 마우스
GLfloat mx, my; // 마우스 좌표
bool lButtonDown;  // 마우스 좌클릭 여부
bool mouseClickEnable = true;  // 마우스 클릭 사용 가능 여부, 기본 true
bool startIntro;  // 카메라 인트로애니메이션 재생 여부, Camera 객체 전용
bool gameStarted; // 게임 시작을 알리는 변수

// 메뉴바 기능
bool playFunc;  // 메뉴바 기능 실행 여부
int funcNumber;  // 메뉴바 기능 구분 숫자
double beatDelay;  // 일부 효과에 사용되는 박자 딜레이
GLfloat beatVal;  // 박자 효과 수치
GLfloat interpolation;  // 박자 효과 보간 값
GLfloat interval, playTime;  // 지정 박자 간격, 지정 기능 실행 시간
GLfloat beatAcc;  // 박자 효과 가속 값
GLfloat functionOperationTime;  // 기능 실행 시간

// 흰 배경
GLfloat whiteTransparent;  // 흰 배경 투명도

// 종료 / 정보창
int ExitOrInfo = 0;  // 0: exit, 1: info
bool setInfo;  // true일 시 정보 표시 활성화

// 로딩
bool LoadingEnd;  // 리소스 로딩 완료 여부

// 여기에 클래스 선언하고 사용
// 메인 오브젝트
Camera cam;
Alcy alcy;
Icon icon[ICON_PART];
Background background;

// 서브 오브젝트
ZZZ zzz[3];
Turntable turntable;
Speaker speaker;
Guitar guitar;
Arm arm;
Light light;
Mic mic;
Neon neon;
Note note;

// UI 오브젝트
UI ui;
Info info;
White white;
Start start;