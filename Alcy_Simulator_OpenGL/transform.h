﻿// 모델 변환
#ifndef MODEL_TRANSLATOR_H
#define MODEL_TRANSLATOR_H
#include "config.h"

void setWindowView();
void initTransform();
void transmit();

extern GLfloat transparent;
extern glm::vec3 cameraPos, cameraDirection, cameraUp, lightPos, objColor;
extern glm::mat4 transformMatrix, view, projection, lightMatrix, scaleMatrix, rotateMatrix, translateMatrix, camMaxtrix;
extern glm::vec3 selectedColor, threshold;

extern unsigned int projectionLocation, viewLocation, modelLocation, viewPosLocation;
extern unsigned int lightPosLocation, lightColorLocation, objColorLocation;
extern unsigned int colorLocation, thresholdLocation;
extern unsigned int transparencyLocation;

#endif
