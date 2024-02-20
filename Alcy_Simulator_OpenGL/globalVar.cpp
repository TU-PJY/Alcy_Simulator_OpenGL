#include "globalVar.h"
#include "screen.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
// 프로젝트 전체 전역 변수 및 클래스

GLfloat mx, my; // 마우스 좌표
GLfloat ratio;  // 화면 종횡비;

bool lButtonDown;  // 마우스 클릭 여부
bool mouseClickEnable;  // 마우스 클릭 사용 가능 여부

// 여기에 클래스 선언하고 사용
Alcy alcy;
Camera cam;
UI ui;