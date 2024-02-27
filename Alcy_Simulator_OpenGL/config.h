﻿#ifndef CONFIG_H
#define CONFIG_H
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4305)
#pragma warning(disable: 4326)
#pragma warning(disable: 4244)
#include <windows.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <random>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp> 
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "stb_image.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#define ALCY_PART 8  // 알키 파트 개수
#define UI_PART 6  // ui 파트 개수
#define ICON_PART 4  // 메뉴 아이콘 개수
#define INTRO 1  // 1: true, 0: false

extern bool LoadingEnd;  // 로고가 출력된 이후부터 리소스 로딩을 시작한다.

using namespace std;


#endif
