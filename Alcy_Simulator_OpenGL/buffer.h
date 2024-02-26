// 버퍼 초기화
#ifndef MODEL_BUFFER_H
#define MODEL_BUFFER_H
#include "config.h"

void vertexInput();
void parameteri();

extern GLuint VBO;  // MODEL_COUNT는 config.h에 정의되어있음
extern GLuint VAO_ALCY[ALCY_PART];  // 알키 전용 독립 VAO
extern GLuint VAO_UI[UI_PART]; // UI 전용 독립 VAO

#endif
