﻿#include "gameVariable.h"
#include "Alcy.h"
#include "Camera.h"
// 여기에 클래스 선언하고 사용

// 마우스
GLfloat mx, my; 

GLfloat ratio;  // 화면 종횡비;

// 커서
bool lButtonDown;
bool cursorEnable = true;  // 커서 보임 여부
bool handEnable = false;  // 손커서 전환 여부
GLfloat handX;  // 손커서 x좌표, 쓰다듬기 시 사용
GLfloat handNum;  // 손 커서 애니메이션에 사용되는 수치

// 나가기 아이콘
bool exitEnable;
GLfloat exitTransparent; // 나가기 아이콘 투명도

//팁
bool tipEnable = true;
GLfloat tipTransparent = 1.0f;

Alcy alcy;
Camera cam;