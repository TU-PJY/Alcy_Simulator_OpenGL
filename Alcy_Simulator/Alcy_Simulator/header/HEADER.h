#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4244)
#pragma warning(disable: 4326)
#pragma warning(disable: 4305)

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp> 
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include "../framework/FWL.h"
#include "stb_image.h"

#include "fmod.hpp"
#include "fmod_errors.h"

#include <windows.h>
#include <stdio.h>   // Header File For Standard Input/Output
#include <stdarg.h>  // Header File For Variable Argument Routines
#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <algorithm>
#include <cmath>
#include <random>
#include <numeric>


// 전역 변수 여기에 선언

//프레임 워크
extern FWL fw;

// 화면 크기
extern int WIDTH, HEIGHT;

// GPU 제조사
extern std::string vendor;

// 종횡비
extern GLfloat ratio;

//마우스 위치
extern GLfloat mx, my;


// layer names
enum main_layers {
	background_layer,
	prop_layer1,
	alcy_layer,
	prop_layer2,
	prop_layer3,
	ui_layer,
	cursor_layer,
};

enum popup_layers {
	layer1,
	layer2,
	layer3,
	layer4,
};