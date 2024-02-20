// 모델 변환 
#include "translate.h"
#include "texture.h"
#include "screen.h"
#include "shader.h"
#include "buffer.h"
#include "Alcy.h"
#include "gameVariable.h"

void finishTransformUI(int idx) {  // 변환 전달 
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

	glBindVertexArray(VAO_UI[idx]);  // 각 모델마다 지정된 VAO만 사용
}

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
	view = translate(view, vec3(camX * ratio, camY, 0));
	view = rotate(view, radians(camRot), vec3(0.0, 0.0, 1.0));
	projection = ortho(-1.0 * ratio / zoom, 1.0 * ratio / zoom, -1.0 / zoom, 1.0 / zoom, -100.0, 100.0);
}

void setTransformUI(int idx) {  // 변환 세팅
	using namespace glm;
	transformMatrix = mat4(1.0f);  // 최종 행렬

	scaleMatrix = mat4(1.0f);  // 신축 행렬
	rotateMatrix = mat4(1.0f);  // 회전 행렬
	translateMatrix = mat4(1.0f);  // 이동 행렬

	selectedColor = vec3(0.0, 0.0, 0.0);  // 클리핑 대상 색상
	threshold = vec3(0.0, 0.0, 0.0);  // 클리핑 임계 영역
	transparent = 1.0f;

	switch (idx) {  // 변환 추가 
	case 0:
		scaleMatrix = scale(scaleMatrix, vec3(2.0 * ratio / zoom, 2.0 / zoom, 0.0));
		translateMatrix = translate(translateMatrix, vec3(-camX * ratio / 4, -camY / 4, -0.001));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.85, 0.0);
		break;

	case 1:  // 팁
		scaleMatrix = scale(scaleMatrix, vec3(0.5 / zoom, 0.5 / zoom, 0.0));
		translateMatrix = translate(translateMatrix, vec3((-0.7 - camX) * ratio, 0.5 - camY, 0.0005));
		rotateMatrix = rotate(rotateMatrix, radians(-camRot), vec3(0.0, 0.0, 1.0));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.8, 0.0);
		transparent = tipTransparent;
		break;

	case 2:  // 나가기 표시
		scaleMatrix = scale(scaleMatrix, vec3(0.5 / zoom, 0.5 / zoom, 1.0));
		translateMatrix = translate(translateMatrix, vec3((0.0 - camX) * ratio, 0.0 - camY, 0.0005));
		rotateMatrix = rotate(rotateMatrix, radians(-camRot), vec3(0.0, 0.0, 1.0));

		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.8, 0.0);
		transparent = exitTransparent;
		break;

	case 3:  // cursor, 항상 맨 마지막에 출력
		scaleMatrix = scale(scaleMatrix, vec3(0.1 / zoom, 0.1 / zoom, 1.0));
		if(!alcy.touchEnable)
			translateMatrix = translate(translateMatrix, vec3((mx - camX) * ratio, my - camY, 0.001));
		else if(alcy.touchEnable)
			translateMatrix = translate(translateMatrix, vec3((handX - camX) * ratio, 0.3 - camY, 0.001));
		selectedColor = vec3(0.0, 1.0, 0.0);
		threshold = vec3(0.0, 0.85, 0.0);
		break;
	}

	transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
}

void modelOutputUI(int idx) {  // 모델 출력 
	switch (idx) {
	case 0:
		glBindTexture(GL_TEXTURE_2D, back);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 1:  // tip
		glBindTexture(GL_TEXTURE_2D, tip);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 2:  // icon
		glBindTexture(GL_TEXTURE_2D, icon[0]);  // exit button
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;

	case 3:  // cursor
		if(!handEnable)
			glBindTexture(GL_TEXTURE_2D, cursor[0]);
		else
			glBindTexture(GL_TEXTURE_2D, cursor[1]);
		if(camRot == 0 && !camR && !camL)  // 카메라 회전 시 커서를 숨김
			glDrawArrays(GL_TRIANGLES, 0, 6);
		break;
	}
}
