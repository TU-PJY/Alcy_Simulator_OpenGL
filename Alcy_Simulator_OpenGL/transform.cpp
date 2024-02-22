// 모델 변환 
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "shader.h"
#include "buffer.h"
#include "Camera.h"
#include "globalVar.h"

glm::vec3 cameraPos, cameraDirection, cameraUp, lightPos, objColor;
glm::mat4 transformMatrix, view, projection, lightMatrix, scaleMatrix, rotateMatrix, translateMatrix, camMaxtrix;
glm::vec3 selectedColor, threshold;
GLfloat transparent;

unsigned int projectionLocation, viewLocation, modelLocation, viewPosLocation;
unsigned int lightPosLocation, lightColorLocation, objColorLocation;
unsigned int colorLocation, thresholdLocation;
unsigned int transparencyLocation;

void setWindowView() {  // 시점 세팅
	using namespace glm;
	view = mat4(1.0f);
	cameraPos = vec3(0.0f, 0.0f, 1.0f);
	cameraDirection = vec3(0.0f, 0.0f, 0.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
	projection = mat4(1.0f);
	ratio = 1.0 * WIDTH / HEIGHT;  // 화면 비율을 구하여 모델이 제대로 나오도록 함
	// X축 변환에 곱해야함.

	view = lookAt(cameraPos, cameraDirection, cameraUp);
	view = translate(view, vec3(cam.camX * ratio, cam.camY, 0));
	view = rotate(view, radians(cam.camRot), vec3(0.0, 0.0, 1.0));
	projection = ortho(-1.0 * ratio / cam.zoom, 1.0 * ratio / cam.zoom, -1.0 / cam.zoom, 1.0 / cam.zoom, -100.0, 100.0);
}

void initTransform() {
    using namespace glm;
    transformMatrix = mat4(1.0f);  // 최종 행렬

    scaleMatrix = mat4(1.0f);  // 신축 행렬
    rotateMatrix = mat4(1.0f);  // 회전 행렬
    translateMatrix = mat4(1.0f);  // 이동 행렬

    selectedColor = vec3(0.0, 0.0, 0.0);  // 클리핑 대상 색상
    threshold = vec3(0.0, 0.0, 0.0);  // 클리핑 임계 영역
    transparent = 1.0f;
}

void transmit() {  // transmit transform data to glsl
    transparencyLocation = glGetUniformLocation(ID, "transparency");
    glUniform1f(transparencyLocation, transparent);

    projectionLocation = glGetUniformLocation(ID, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

    viewLocation = glGetUniformLocation(ID, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    objColorLocation = glGetUniformLocation(ID, "objectColor");
    glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

    viewPosLocation = glGetUniformLocation(ID, "viewPos"); // viewPos 값 전달: 카메라 위치
    glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

    modelLocation = glGetUniformLocation(ID, "model"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(transformMatrix)); // modelTransform 변수에 변환 값 적용하기
}
