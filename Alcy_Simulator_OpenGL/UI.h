#ifndef UI_H
#define UI_H
#include "config.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "translate.h"
#include "gameVariable.h"
//#include "Alcy.h"
//#include "Camera.h"

class Alcy;
class Cam;

class UI {
public:
	// 커서
	bool cursorEnable;  // 커서 보임 여부
	bool handEnable;  // 손커서 전환 여부
	GLfloat handX;  // 손커서 x좌표, 쓰다듬기 시 사용
	GLfloat handNum;  // 손 커서 애니메이션에 사용되는 수치

	// 나가기 아이콘
	bool exitEnable;
	GLfloat exitTransparent; // 나가기 아이콘 투명도

	//팁
	bool tipEnable;
	GLfloat tipTransparent;

	UI() {
		cursorEnable = true;
		tipEnable = true;
		tipTransparent = 1.0f;
	}
    // ui
    void exitGame() {  // esc를 길게 눌러 게임 종료
        if (exitEnable) {
            exitTransparent += fs / 6;
            if (exitTransparent > 1.0) {
                exitTransparent = 1.0;
                glutDestroyWindow(1);
            }
        }
        else {
            exitTransparent -= fs / 6;
            if (exitTransparent < 0) {
                exitTransparent = 0;
            }
        }
    }

    void updateTip() {  // 팁 on/off
        if (tipEnable) {
            tipTransparent += fs / 3;
            if (tipTransparent > 1.0)
                tipTransparent = 1.0;
        }
        else {
            tipTransparent -= fs / 3;
            if (tipTransparent < 0.0)
                tipTransparent = 0.0;
        }
    }

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

		objColorLocation = glGetUniformLocation(ID, "objectColor");
		glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

		viewPosLocation = glGetUniformLocation(ID, "viewPos"); // viewPos 값 전달: 카메라 위치
		glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

		modelLocation = glGetUniformLocation(ID, "model"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(transformMatrix)); // modelTransform 변수에 변환 값 적용하기

		glBindVertexArray(VAO_UI[idx]);  // 각 모델마다 지정된 VAO만 사용
	}

	void setTransform(int idx) {  // 변환 세팅
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
			scaleMatrix = scale(scaleMatrix, vec3(2.0 * ratio / cam.zoom, 2.0 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(-cam.camX * ratio / 4, -cam.camY / 4, -0.001));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.85, 0.0);
			break;

		case 1:  // 팁
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3((-0.7 - cam.camX) * ratio, 0.5 - cam.camY, 0.0005));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.8, 0.0);
			transparent = tipTransparent;
			break;

		case 2:  // 나가기 표시
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 1.0));
			translateMatrix = translate(translateMatrix, vec3((0.0 - cam.camX) * ratio, 0.0 - cam.camY, 0.0005));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));

			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.8, 0.0);
			transparent = exitTransparent;
			break;

		case 3:  // cursor, 항상 맨 마지막에 출력
			scaleMatrix = scale(scaleMatrix, vec3(0.1 / cam.zoom, 0.1 / cam.zoom, 1.0));
			if (!alcy.touchEnable)
				translateMatrix = translate(translateMatrix, vec3((mx - cam.camX) * ratio, my - cam.camY, 0.001));
			else if (alcy.touchEnable)
				translateMatrix = translate(translateMatrix, vec3((handX - cam.camX) * ratio, 0.3 - cam.camY, 0.001));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.85, 0.0);
			break;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
	}

	void modelOutput(int idx) {  // 모델 출력 
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
			if (!handEnable)
				glBindTexture(GL_TEXTURE_2D, cursor[0]);
			else
				glBindTexture(GL_TEXTURE_2D, cursor[1]);
			if (cam.camRot == 0 && !cam.camR && !cam.camL)  // 카메라 회전 시 커서를 숨김
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
	}

};

extern UI ui;

#endif