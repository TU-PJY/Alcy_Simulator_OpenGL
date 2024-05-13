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


// ���� ���� ���⿡ ����

//������ ��ũ
extern FWL fw;

// ȭ�� ũ��
extern int WIDTH, HEIGHT;

// GPU ������
extern std::string vendor;

// ��Ⱦ��
extern GLfloat ratio;

//���콺 ��ġ
extern GLfloat mx, my;


// layer names
enum main_layers {
	main_layer1,
	main_layer2,
	main_layer3,
	main_layer4,
	main_layer5,
	main_layer6,
	main_layer7,
};

enum popup_layers {
	sub_layer1,
	sub_layer2,
	sub_layer3,
	sub_layer4,
};