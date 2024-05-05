// 셰이더 프로그램 생성
#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H
#include "config.h"

char* loadBuffer(const char* file);
void makeShaderProgram();

extern GLchar* vertexSource, * fragmentSource;
extern GLuint vertexShader, fragmentShader;
extern GLuint ID;

#endif
