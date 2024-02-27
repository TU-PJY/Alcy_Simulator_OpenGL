#ifndef UI_H
#define UI_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "Info.h"

enum ui_name {
	press_,
	title_,
	tip_,
	icon_,
	bar_,
	cursor_,
};

class UI {
public:
	GLuint VAO_UI;

	unsigned int press;  // press space 안내 문구
	unsigned int cursor[3];  // 배경, 커서
	unsigned int bar;  //메뉴바
	unsigned int uiIcon[3];  // 아이콘
	unsigned int tip;  // 팁
	unsigned int title;  // 타이틀

	int channel;
	int titleW = 1500, titleH = 1500;
	int cursorW = 200, cursorH = 200;
	int iconW = 512, iconH = 512;
	int tipW = 500, tipH = 500;
	int barW = 512, barH = 512;

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

    void exitGame() {  // esc를 길게 눌러 게임 종료, 정보창 띄위기와 공용
        if (exitEnable || infoEnable) {
            exitTransparent += fs / 6;  // 종료 아이콘이 완전히 나타나면 게임을 종료한다.
            if (exitTransparent > 1.0) {
                exitTransparent = 1.0;

				if (ExitOrInfo == 0)  // 종료
					glutDestroyWindow(1);

				if (ExitOrInfo == 1) {  // 정보창 활성화
					setInfo = true;
					infoEnable = false;
					exitTransparent = 0.0;

					// 정보창 초기화
					info.infoTransparent = 1.0;
					info.size = 0.5;
					info.acc = 0.5;
				}
            }
        }

        else {
            exitTransparent -= fs / 6;  // 종료 도중 그만 둘 경우 아이콘은 다시 투명해진다.
            if (exitTransparent < 0)
                exitTransparent = 0;
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

	void setBuffer() {
		glGenVertexArrays(1, &VAO_UI);
		glBindVertexArray(VAO_UI);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		// press space
		glGenTextures(1, &press);
		glBindTexture(GL_TEXTURE_2D, press);
		parameteri();
		texture_data = stbi_load("res//ui//press_space.png", &tipW, &tipH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tipW, tipH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
		// title
		glGenTextures(1, &title);
		glBindTexture(GL_TEXTURE_2D, title);
		parameteri();
		texture_data = stbi_load("res//ui//title.png", &titleW, &titleH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, titleW, titleH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// cursor
		glGenTextures(1, &cursor[0]);
		glBindTexture(GL_TEXTURE_2D, cursor[0]);
		parameteri();
		texture_data = stbi_load("res//ui//cursor.png", &cursorW, &cursorH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cursorW, cursorH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// cursor hand
		glGenTextures(1, &cursor[1]);
		glBindTexture(GL_TEXTURE_2D, cursor[1]);
		parameteri();
		texture_data = stbi_load("res//ui//cursor_hand.png", &cursorW, &cursorH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cursorW, cursorH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// cursor finger
		glGenTextures(1, &cursor[2]);
		glBindTexture(GL_TEXTURE_2D, cursor[2]);
		parameteri();
		texture_data = stbi_load("res//ui//cursor_finger.png", &cursorW, &cursorH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cursorW, cursorH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// exit icon
		glGenTextures(1, &uiIcon[0]);
		glBindTexture(GL_TEXTURE_2D, uiIcon[0]);
		parameteri();
		texture_data = stbi_load("res//ui//icon_exit.png", &iconW, &iconH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iconW, iconH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// info icon
		glGenTextures(1, &uiIcon[1]);
		glBindTexture(GL_TEXTURE_2D, uiIcon[1]);
		parameteri();
		texture_data = stbi_load("res//ui//icon_info.png", &iconW, &iconH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iconW, iconH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// tip
		glGenTextures(1, &tip);
		glBindTexture(GL_TEXTURE_2D, tip);
		parameteri();
		texture_data = stbi_load("res//ui//tip.png", &tipW, &tipH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tipW, tipH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// bar
		glGenTextures(1, &bar);
		glBindTexture(GL_TEXTURE_2D, bar);
		parameteri();
		texture_data = stbi_load("res//ui//bar.png", &barW, &barH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, barW, barH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void modelOutput(int idx) {  // 모델 출력 
		switch (idx) {
		case press_:
			if (gameStarted) break;

			glBindTexture(GL_TEXTURE_2D, press);
			if (INTRO == 1)
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case title_:
			if (gameStarted) break;

			glBindTexture(GL_TEXTURE_2D, title);
			if (INTRO == 1)
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case tip_:
			if (tipTransparent == 0.0) break;

			if (gameStarted) {
				glDepthMask(GL_FALSE);
				glBindTexture(GL_TEXTURE_2D, tip);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glDepthMask(GL_TRUE);
			}
			break;

		case icon_:
			if (exitTransparent == 0.0) break;

			glDepthMask(GL_FALSE);
			if(ExitOrInfo == 0)
				glBindTexture(GL_TEXTURE_2D, uiIcon[0]);  // exit icon
			else if(ExitOrInfo == 1)
				glBindTexture(GL_TEXTURE_2D, uiIcon[1]);  // info icon
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDepthMask(GL_TRUE);
			break;

		case bar_:
			if (!gameStarted) break;

			glDepthMask(GL_FALSE);
			glBindTexture(GL_TEXTURE_2D, bar);  // exit icon
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDepthMask(GL_TRUE);

			break;

		case cursor_:
			if (!gameStarted) break;

			if (handEnable)  // 손 커서
				glBindTexture(GL_TEXTURE_2D, cursor[1]);
			else if (fingerEnable)  // 손가락 커서
				glBindTexture(GL_TEXTURE_2D, cursor[2]);
			else // 일반 커서
				glBindTexture(GL_TEXTURE_2D, cursor[0]);

			if (!playFunc && cam.camRot == 0 && !cam.camR && !cam.camL)  // 카메라 회전 시 커서를 숨김
				glDrawArrays(GL_TRIANGLES, 0, 6);
			else if (playFunc)
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
	}

	void setObject(int idx) {  // 변환 세팅
		using namespace glm;

		initTransform();

		switch (idx) {
		case press_:
			if (gameStarted) break;

			scaleMatrix = scale(scaleMatrix, vec3(0.4 / cam.zoom, 0.4 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(0.0, -0.9 / cam.zoom, 0.1));
			transparent = titleTransparent;
			break;

		case title_:
			if (gameStarted) break;

			scaleMatrix = scale(scaleMatrix, vec3(titleSize / cam.zoom, titleSize / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(0.0, titleY, 0.05));
			transparent = titleTransparent;
			break;

		case tip_:
			if (tipTransparent == 0.0 || !gameStarted) break;

			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(-1.0 * ratio_ / cam.zoom + 0.5 / cam.zoom - cam.camX * ratio_, 0.5 / cam.zoom - cam.camY, 1.0));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			transparent = tipTransparent;
			break;

		case icon_:
			if (exitTransparent == 0.0) break;

			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3((0.0 - cam.camX) * ratio_, 0.0 - cam.camY, 1.0));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			transparent = exitTransparent;
			break;

		case bar_:  // 화면 크기에 따라 메뉴바의 최대 크기가 달라짐
			if (!gameStarted) break;

			scaleMatrix = scale(scaleMatrix, vec3((0.2 + menuSizeX) * ratio_ / cam.zoom, (0.01 + menuSizeY) / cam.zoom, 0.0));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			translateMatrix = translate(translateMatrix, vec3(-cam.camX * ratio_, (menuY + menuSizeY / 1.2) / cam.zoom - cam.camY, 1.0));
			transparent = menuTransparent;
			break;

		case cursor_:
			if (!gameStarted) break;

			scaleMatrix = scale(scaleMatrix, vec3(0.15 / cam.zoom, 0.15 / cam.zoom, .0));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			if (lButtonDown && handEnable)  // 쓰다듬을 때는 커서를 강제로 지정된 위치로 변환한다.
				translateMatrix = translate(translateMatrix, vec3((handX - cam.camX), (0.3 - cam.camY), 0.0006));
			else
				translateMatrix = translate(translateMatrix, vec3((mx / cam.zoom - cam.camX) * ratio_, (my / cam.zoom - cam.camY), 1.0));
			break;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
		transmit();

		glBindVertexArray(VAO_UI);  // 각 모델마다 지정된 VAO만 사용

		modelOutput(idx);
	}
};

extern UI ui;

#endif