// OpenGL 기능 함수는 여기에 정의
#ifndef GL_FUNC_H
#define GL_FUNC_H
#include "config.h"

void keyDown(unsigned char KEY, int x, int y);
void keyUp(unsigned char KEY, int x, int y);
void timerOperation(int value);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

#endif
