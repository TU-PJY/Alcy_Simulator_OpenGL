#ifndef UI_H
#define UI_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

enum ui_name {
	background_,
	tip_,
	icon_,
	cursor_,
};

class UI {
public:
	bool handEnable;  // 손커서 전환 여부
	GLfloat handX;  // 손커서 x좌표, 쓰다듬기 시 사용
	GLfloat handNum;  // 손 커서 애니메이션에 사용되는 수치
	bool fingerEnable; // 손가락 커서 전환 여부

	// 나가기 아이콘
	bool exitEnable;
	GLfloat exitTransparent; // 나가기 아이콘 투명도

	//팁
	bool tipEnable;
	GLfloat tipTransparent;

	UI() {
		mouseClickEnable = true;
		tipEnable = true;
		tipTransparent = 1.0f;
	}
    // ui
    void exitGame() {  // esc를 길게 눌러 게임 종료
        if (exitEnable) {
            exitTransparent += fs / 6;  // 종료 아이콘이 완전히 나타나면 게임을 종료한다.
            if (exitTransparent > 1.0) {
                exitTransparent = 1.0;
                glutDestroyWindow(1);
            }
        }
        else {
            exitTransparent -= fs / 6;  // 종료 도중 그만 둘 경우 아이콘은 다시 투명해진다.
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

	void bindVertex(int idx) {
		glBindVertexArray(VAO_UI[idx]);  // 각 모델마다 지정된 VAO만 사용
	}

	void setTransform(int idx) {  // 변환 세팅
		using namespace glm;

		switch (idx) {
		case background_: 
			scaleMatrix = scale(scaleMatrix, vec3(2.0 * ratio / cam.zoom, 2.0 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(-cam.camX * ratio / 4, -cam.camY / 4, -0.001));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.85, 0.0);
			break;

		case tip_:
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3((-0.7 - cam.camX) * ratio, 0.5 - cam.camY, 0.0005));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.8, 0.0);
			transparent = tipTransparent;
			break;

		case icon_:
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 1.0));
			translateMatrix = translate(translateMatrix, vec3((0.0 - cam.camX) * ratio, 0.0 - cam.camY, 0.0005));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));

			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.8, 0.0);
			transparent = exitTransparent;
			break;

		case cursor_:
			if(fingerEnable)
				scaleMatrix = scale(scaleMatrix, vec3(0.15 / cam.zoom, 0.15 / cam.zoom, 1.0));
			else
				scaleMatrix = scale(scaleMatrix, vec3(0.1 / cam.zoom, 0.1 / cam.zoom, 1.0));

			if (lButtonDown && handEnable)  // 쓰다듬을 때는 커서를 강제로 지정된 위치로 변환한다.
				translateMatrix = translate(translateMatrix, vec3((handX - cam.camX) * ratio, 0.3 - cam.camY, 0.001));
			else
				translateMatrix = translate(translateMatrix, vec3((mx - cam.camX) * ratio, my - cam.camY, 0.001));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.85, 0.0);
			break;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
	}

	void modelOutput(int idx) {  // 모델 출력 
		switch (idx) {
		case background_:
			glBindTexture(GL_TEXTURE_2D, back);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case tip_:
			glBindTexture(GL_TEXTURE_2D, tip);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case icon_:
			glBindTexture(GL_TEXTURE_2D, icon[0]);  // exit icon
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case cursor_:
			if (handEnable)  // 손 커서
				glBindTexture(GL_TEXTURE_2D, cursor[1]);
			else if (fingerEnable)  // 손가락 커서
				glBindTexture(GL_TEXTURE_2D, cursor[2]);
			else // 일반 커서
				glBindTexture(GL_TEXTURE_2D, cursor[0]);

			if (cam.camRot == 0 && !cam.camR && !cam.camL)  // 카메라 회전 시 커서를 숨김
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
	}

};

extern UI ui;

#endif