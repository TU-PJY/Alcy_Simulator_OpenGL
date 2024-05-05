// OpenGL 기능 함수
#pragma once
#include "HEADER.h"


void key_down(unsigned char KEY, int x, int y);
void key_up(unsigned char KEY, int x, int y);
void mouse_button(int button, int state, int x, int y);
void mouse_passive_motion(int x, int y);
void mouse_motion(int x, int y);
void mouse_wheel(int button, int dir, int x, int y);