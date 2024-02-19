// 버퍼 초기화
#ifndef MODEL_BUFFER_H
#define MODEL_BUFFER_H
#include "config.h"

void setBuffer(int idx);
void vertexInput(int idx);
void setTexture();

extern GLuint VAO[PLATE_COUNT], VBO;  // MODEL_COUNT는 config.h에 정의되어있음

#endif
