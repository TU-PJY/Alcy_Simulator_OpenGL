#include "config.h"
#include "translate.h"
// 여기에 변환 변수 선언 후 사용
GLfloat transparent;
glm::vec3 cameraPos, cameraDirection, cameraUp, lightPos, objColor;
glm::mat4 transformMatrix, view, projection, lightMatrix, scaleMatrix, rotateMatrix, translateMatrix, camMaxtrix;
glm::vec3 selectedColor, threshold;

unsigned int projectionLocation, viewLocation, modelLocation, viewPosLocation;
unsigned int lightPosLocation, lightColorLocation, objColorLocation;
unsigned int colorLocation, thresholdLocation;
int transparencyLocation;