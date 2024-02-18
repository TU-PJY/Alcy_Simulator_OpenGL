// 모델 변환 
#include "translate.h"
#include "texture.h"
#include "screen.h"

extern GLuint ID;
extern GLuint VAO[PLATE_COUNT];  // MODEL_COUNT는 config.h에 정의되어있음

// 카메라 
extern GLfloat ratio;  //  종횡비
extern GLfloat camX, camY;  // 카메라 위치
extern GLfloat camRot;  // 카메라 각도
extern GLfloat mx, my;  // 마우스 위치, mx에는 ratio를 곱해야 올바른 값이 나옴
extern GLfloat zoom;
extern bool camR, camL;  // 방향 별 카메라 회전 여부

// 커서
extern bool cursorEnable;
extern bool handEnable;

// 알키 관련 변수
extern int dir;  // 알키 바라보는 방향
extern bool blinkEnable;  // 알키 눈 깜빡임 여부

// 알키 머리 움직임
extern GLfloat headPos;

// 이미지 투명 값
extern GLfloat transparent;

glm::vec3 cameraPos, cameraDirection, cameraUp, lightPos, objColor;
glm::mat4 transformMatrix, view, projection, lightMatrix, scaleMatrix, rotateMatrix, translateMatrix, camMaxtrix;
glm::vec3 selectedColor, threshold;

unsigned int projectionLocation, viewLocation, modelLocation, viewPosLocation;
unsigned int lightPosLocation, lightColorLocation, objColorLocation;
unsigned int colorLocation, thresholdLocation;
int transparencyLocation;


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
	cameraPos = vec3(0.0f, 0.0f, 1.0f);
	cameraDirection = vec3(0.0f, 0.0f, 0.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
	projection = mat4(1.0f);
	ratio = 1.0 * WIDTH / HEIGHT;  // 화면 비율을 구하여 모델이 제대로 나오도록 함
	// 신축, 회전 변환 및 X축 변환에 곱해야함.

	view = lookAt(cameraPos, cameraDirection, cameraUp);
	view = translate(view, vec3(camX * ratio, camY, 0));
	view = rotate(view, radians(camRot), vec3(0.0, 0.0, 1.0));
	projection = ortho(-1.0 * ratio / zoom, 1.0 * ratio / zoom, -1.0 / zoom, 1.0 / zoom, -100.0, 100.0);
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
		scaleMatrix = scale(scaleMatrix, vec3(2.0 * ratio / zoom, 2.0 / zoom, 0.0));
		translateMatrix = translate(translateMatrix, vec3(-camX * ratio / 4, -camY / 4, 0.0));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.85, 0.0);
		break;

	case 1:  // tail
		translateMatrix = translate(translateMatrix, vec3(-0.2 * ratio, -0.75, -0.00003));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.8, 0.0);
		break;

	case 2:  // body
		translateMatrix = translate(translateMatrix, vec3(0.0, -0.75, -0.00002));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.8, 0.0);
		break;

	case 3:  // hair
		translateMatrix = translate(translateMatrix, vec3(0.0, -0.75, -0.00001));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.8, 0.0);
		break;

	case 4:  // head
		translateMatrix = translate(translateMatrix, vec3(headPos * ratio, 0.12, 0.0));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.8, 0.0);
		break;

	case 5:  // eye
		if(camRot == 0 && !camR && !camL)
			translateMatrix = translate(translateMatrix, vec3((headPos - (camX / 4)) * ratio, 0.12 - (camY / 4), 0.00001));
		else
			translateMatrix = translate(translateMatrix, vec3(headPos * ratio, 0.12, 0.00001));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.9, 0.0);
		break;
	
	case 6:  // dot
		if (camRot == 0 && !camR && !camL)
			translateMatrix = translate(translateMatrix, vec3((headPos - (camX / 2.5)) * ratio, 0.12 - (camY / 2), 0.00003));
		else
			translateMatrix = translate(translateMatrix, vec3(headPos * ratio, 0.12, 0.00003));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 1.0, 0.0);
		break;
	
	case 7:  // brow
		if (camRot == 0 && !camR && !camL)
			translateMatrix = translate(translateMatrix, vec3(headPos * ratio, 0.13 - (camY / 4), 0.00003));
		else
			translateMatrix = translate(translateMatrix, vec3(headPos * ratio, 0.12, 0.00003));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.9, 0.0);
		break;

	case 8:  // blink
		translateMatrix = translate(translateMatrix, vec3(headPos * ratio, 0.12, 0.00004));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.7, 0.0);
		break;

	case PLATE_COUNT - 1:  // cursor, 항상 맨 마지막에 출력
		scaleMatrix = scale(scaleMatrix, vec3(0.1 / zoom, 0.1 / zoom, 1.0));
		translateMatrix = translate(translateMatrix, vec3(mx * ratio - camX * ratio, my - camY, 0.001));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.85, 0.0);
		break;
	}

	transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
}

void modelOutput(int idx) {  // 모델 출력 
	switch (idx) {
	case 0:  // background
		glBindTexture(GL_TEXTURE_2D, back);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 1:  // tail
		glBindTexture(GL_TEXTURE_2D, alcyTail);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 2:  // body
		glBindTexture(GL_TEXTURE_2D, alcyBody);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 3:  // hair
		glBindTexture(GL_TEXTURE_2D, alcyHair);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 4:  // head
		if (camRot == 0 && !camR && !camL) {
			switch (dir) {
			case l:
				glBindTexture(GL_TEXTURE_2D, alcyHead[1]);  // head left
				break;
			case r:
				glBindTexture(GL_TEXTURE_2D, alcyHead[2]);  // head right
				break;
			case m:
				glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head middle
				break;
			}
		}
		else   // 카메라 회전 시 앞을 보도록 함
			glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head middle
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 5:  // eye
		if (camRot == 0 && !camR && !camL) {
			switch (dir) {
			case l:
				glBindTexture(GL_TEXTURE_2D, eye[1]);  // eye left
				break;
			case r:
				glBindTexture(GL_TEXTURE_2D, eye[2]);  // eye right
				break;
			case m:
				glBindTexture(GL_TEXTURE_2D, eye[0]);  // eye middle
				break;
			}
		}
		else  // 카메라 회전 시 앞을 보도록 함
			glBindTexture(GL_TEXTURE_2D, eye[0]);  // eye middle

		if (!blinkEnable)  // 눈을 깝빡이지 않을 때만 출력한다.
			glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 6:  // dot
		if (camRot == 0 && !camR && !camL) {
			switch (dir) {
			case l:
				glBindTexture(GL_TEXTURE_2D, dot[1]);  // eye left
				break;
			case r:
				glBindTexture(GL_TEXTURE_2D, dot[2]);  // eye right
				break;
			case m:
				glBindTexture(GL_TEXTURE_2D, dot[0]);  // eye middle
				break;
			}
		}
		else  // 카메라 회전 시 앞을 보도록 함
			glBindTexture(GL_TEXTURE_2D, dot[0]);  // eye middle

		if (!blinkEnable)  // 눈을 깜빡이지 않을 때만 출력한다.
			glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 7:  // brow
		if (camRot == 0 && !camR && !camL) {
			switch (dir) {
			case l:
				glBindTexture(GL_TEXTURE_2D, brow[1]);  // brow left
				break;
			case r:
				glBindTexture(GL_TEXTURE_2D, brow[2]);  // brow right
				break;
			case m:
				glBindTexture(GL_TEXTURE_2D, brow[0]);  // brow middle
				break;
			}
		}
		else  // 카메라 회전 시 앞을 보도록 함
			glBindTexture(GL_TEXTURE_2D, brow[0]);  // brow middle
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 8:  // blink
		if (camRot == 0 && !camR && !camL) {
			switch (dir) {
			case l:
				glBindTexture(GL_TEXTURE_2D, blink[1]);  // blink left
				break;
			case r:
				glBindTexture(GL_TEXTURE_2D, blink[2]);  // blink right
				break;
			case m:
				glBindTexture(GL_TEXTURE_2D, blink[0]);  // blink middle
				break;
			}
		}
		else 
			glBindTexture(GL_TEXTURE_2D, blink[0]);  // blink middle
		if(blinkEnable)  // true일 때만 출력
			glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case PLATE_COUNT - 1:  // cursor
		if(!handEnable)
			glBindTexture(GL_TEXTURE_2D, cursor[0]);
		else
			glBindTexture(GL_TEXTURE_2D, cursor[1]);
		if(camRot == 0 && !camR && !camL)  // 카메라 회전 시 커서를 숨김
			glDrawArrays(GL_TRIANGLES, 0, 6);
		break;
	}
}
