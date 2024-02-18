// 모델 변환 
#include "translate.h"
#include "texture.h"
#include "screen.h"

extern GLuint ID;
extern GLuint VAO[IMAGE_COUNT];  // MODEL_COUNT는 config.h에 정의되어있음

// 카메라 컨트롤 변수
extern GLfloat camX, camY;
extern GLfloat mx, my;

// 이미지 컨트롤 변수



glm::vec3 cameraPos, cameraDirection, cameraUp, lightPos, objColor;
glm::mat4 transformMatrix, view, projection, lightMatrix, scaleMatrix, rotateMatrix, translateMatrix, camMaxtrix;
glm::vec3 selectedColor, threshold;
GLfloat ratio, transparent;

unsigned int projectionLocation, viewLocation, modelLocation, viewPosLocation;
unsigned int lightPosLocation, lightColorLocation, objColorLocation;
int transparencyLocation;
unsigned int colorLocation, thresholdLocation;


void finishTransform(int idx) {  // 변환 전달 
	colorLocation = glGetUniformLocation(ID, "targetColor");
	glUniform3f(colorLocation, selectedColor.r, selectedColor.g, selectedColor.b);

	thresholdLocation = glGetUniformLocation(ID, "colorThreshold");
	glUniform3f(thresholdLocation, threshold.r, threshold.g, threshold.b);

	transparencyLocation = glGetUniformLocation(ID, "transparency");
	glUniform1f(transparencyLocation, transparent);

	projectionLocation = glGetUniformLocation(ID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	viewLocation = glGetUniformLocation(ID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	objColorLocation = glGetUniformLocation(ID, "objectColor"); // object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	viewPosLocation = glGetUniformLocation(ID, "viewPos"); // viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	modelLocation = glGetUniformLocation(ID, "model"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(transformMatrix)); // modelTransform 변수에 변환 값 적용하기

	glBindVertexArray(VAO[idx]);  // 각 모델마다 지정된 VAO만 사용
}


void setWindowView() {  // 시점 세팅
	using namespace glm;
	view = mat4(1.0f);
	cameraPos = vec3(0.0f, 0.0f, 30.0f);
	cameraDirection = vec3(0.0f, 0.0f, 0.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
	projection = mat4(1.0f);
	ratio = 1.0 * WIDTH / HEIGHT;  // 화면 비율을 구하여 모델이 제대로 나오도록 함

	view = lookAt(cameraPos, cameraDirection, cameraUp);
	//view = translate(view, vec3(camX, camY, 0.0));
	//projection = perspective(radians(1.0f), ratio, 0.1f, 100.0f);
	projection = ortho(-WIDTH, WIDTH, -HEIGHT, HEIGHT, -100, 100);
}

void setTransform(int idx) {  // 변환 세팅
	using namespace glm;
	transformMatrix = mat4(1.0f);  // 최종 행렬

	scaleMatrix = mat4(1.0f);  // 신축 행렬
	rotateMatrix = mat4(1.0f);  // 회전 행렬
	translateMatrix = mat4(1.0f);  // 이동 행렬

	selectedColor = vec3(0.0, 0.0, 0.0);  // 클리핑 대상 색상
	threshold = vec3(0.0, 0.0, 0.0);  // 클리핑 임계 영역
	transparent = 1.0f;  // 이미지 투명도

	switch (idx) {  // 변환 추가 
	case 0: // background
		scaleMatrix = scale(scaleMatrix, vec3(4.0, 2.0, 0.0));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.8, 0.0);
		break;

	case 1:  // tail
		translateMatrix = translate(translateMatrix, vec3(-0.1, -0.22, -0.002));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.7, 0.0);
		break;

	case 2:  // body
		translateMatrix = translate(translateMatrix, vec3(0.0, -0.22, -0.001));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.7, 0.0);
		break;

	case 3:  // head
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.7, 0.0);
		break;
	
	case IMAGE_COUNT - 1:
		//scaleMatrix = scale(scaleMatrix, vec3(0.1, 0.1, 1.0));
		translateMatrix = translate(translateMatrix, vec3(mx, my, 1.0));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.7, 0.0);
		break;
	}

	transformMatrix = scaleMatrix * rotateMatrix * translateMatrix;  // 최종 변환
}

void modelOutput(int idx) {  // 모델 출력 
	switch (idx) {
	case 0:
		glBindTexture(GL_TEXTURE_2D, back);
		break;
	case 1:
		glBindTexture(GL_TEXTURE_2D, alcyTail);
		break;
	case 2:
		glBindTexture(GL_TEXTURE_2D, alcyBody);
		break;
	case 3:
		glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head_middle
		break;
	case IMAGE_COUNT - 1:
		glBindTexture(GL_TEXTURE_2D, cursor);
		break;
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
