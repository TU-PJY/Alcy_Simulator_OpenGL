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
	title_,
	tip_,
	icon_,
	bar_,
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

	// info 아이콘
	bool infoEnable;
	GLfloat infoTransparent;

	//팁
	bool tipEnable;
	GLfloat tipTransparent;

	// 타이틀
	GLfloat titleY, titleTransparent, titleSize;

	// 게임 시작
	bool intro;  // 인트로 애니메이션

	bool menuEnable;  // 메뉴 활성화 여부
	bool menuOpened;  // 메뉴 아이콘 사용 가능 여부
	GLfloat menuY;  // 메뉴 높이
	GLfloat menuSizeX;  // 메뉴 크기
	GLfloat menuSizeY;
	GLfloat menuAcc;  // 메뉴를 열 때 사용하는 가속값
	GLfloat menuTransparent;

	UI() {
		mouseClickEnable = true;
		tipEnable = true;
		tipTransparent = 1.0f;

		titleTransparent = 1.0f;
		titleSize = 1.3f;
		titleY = 0.75;

		menuTransparent = 1.0f;
		menuY = -0.97;
	}

    // ui
	void startGame() {
		if (!gameStarted && intro) {
			titleTransparent -= fs / 5;
			titleY += fs / 30;
			titleSize += fs / 20;

			if (titleTransparent < 0)   // 완전히 투명해지면 더 이상 애니메이션이 진행되지 않는다.
				intro = false;
		}
	}

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

	void updateMenu() {
		if (menuEnable) {
			menuSizeX += menuAcc * 2 * fs;
			menuSizeY += menuAcc * fs;
			menuTransparent -= menuAcc / 2 * fs;

			menuAcc -= fs / 25;

			if (menuAcc < 0) {
				menuTransparent = 0.743;
				menuSizeX = 1.02;
				menuSizeY = 0.51;
				menuAcc = 0;
				menuOpened = true;
			}
		}

		else {
			menuSizeX -= menuAcc * 2 * fs;
			menuSizeY -= menuAcc * fs;
			menuTransparent += menuAcc / 2 * fs;

			menuAcc -= fs / 25;

			if (menuAcc < 0) {
				menuAcc = 0;
				menuSizeX = 0;
				menuSizeY = 0;
				menuTransparent = 1.0;
			}
		}
	}

	void bindVertex(int idx) {
		glBindVertexArray(VAO_UI[idx]);  // 각 모델마다 지정된 VAO만 사용
	}

	void setTransform(int idx) {  // 변환 세팅
		using namespace glm;

		switch (idx) {
		case title_:
			scaleMatrix = scale(scaleMatrix, vec3(titleSize / cam.zoom, titleSize / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(0.0, titleY, 0.05));
			transparent = titleTransparent;
			break;

		case tip_:
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(-1.0 * ratio_ + 0.5 - cam.camX * ratio_, 0.5 - cam.camY, 0.0005));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			transparent = tipTransparent;
			break;

		case icon_:
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3((0.0 - cam.camX) * ratio_, 0.0 - cam.camY, 0.001));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			transparent = exitTransparent;
			break;

		case bar_:  // 화면 크기에 따라 메뉴바의 최대 크기가 달라짐
			scaleMatrix = scale(scaleMatrix, vec3((0.2 + menuSizeX) * ratio_ / cam.zoom, (0.01 + menuSizeY) / cam.zoom, 0.0));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			translateMatrix = translate(translateMatrix, vec3(-cam.camX * ratio_, (menuY + menuSizeY / 1.2) / cam.zoom - cam.camY, 0.1));
			transparent = menuTransparent;
			break;

		case cursor_:
			scaleMatrix = scale(scaleMatrix, vec3(0.15 / cam.zoom, 0.15 / cam.zoom, .0));

			if (lButtonDown && handEnable)  // 쓰다듬을 때는 커서를 강제로 지정된 위치로 변환한다.
				translateMatrix = translate(translateMatrix, vec3((handX - cam.camX), (0.3 - cam.camY), 0.0006));
			else
				translateMatrix = translate(translateMatrix, vec3((mx / cam.zoom - cam.camX) * ratio_, (my / cam.zoom - cam.camY), 1.0));
			break;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
	}

	void modelOutput(int idx) {  // 모델 출력 
		switch (idx) {
		case title_:
			glBindTexture(GL_TEXTURE_2D, title);
			if(INTRO == 1)
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case tip_:
			if (gameStarted) {
				glDepthMask(GL_FALSE);
				glBindTexture(GL_TEXTURE_2D, tip);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glDepthMask(GL_TRUE);
			}
			break;

		case icon_:
			glDepthMask(GL_FALSE);
			glBindTexture(GL_TEXTURE_2D, uiIcon[0]);  // exit icon
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDepthMask(GL_TRUE);
			break;

		case bar_:
			if (gameStarted) {
				glDepthMask(GL_FALSE);
				glBindTexture(GL_TEXTURE_2D, bar);  // exit icon
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glDepthMask(GL_TRUE);
			}
			break;

		case cursor_:
			if (handEnable)  // 손 커서
				glBindTexture(GL_TEXTURE_2D, cursor[1]);
			else if (fingerEnable)  // 손가락 커서
				glBindTexture(GL_TEXTURE_2D, cursor[2]);
			else // 일반 커서
				glBindTexture(GL_TEXTURE_2D, cursor[0]);

			if (cam.camRot == 0 && !cam.camR && !cam.camL && gameStarted)  // 카메라 회전 시 커서를 숨김
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
	}
};

extern UI ui;

#endif