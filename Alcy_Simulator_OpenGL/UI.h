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

	unsigned int press;  // press space �ȳ� ����
	unsigned int cursor[3];  // ���, Ŀ��
	unsigned int bar;  //�޴���
	unsigned int uiIcon[3];  // ������
	unsigned int tip;  // ��
	unsigned int title;  // Ÿ��Ʋ

	int channel;
	int titleW = 1500, titleH = 1500;
	int cursorW = 200, cursorH = 200;
	int iconW = 512, iconH = 512;
	int tipW = 500, tipH = 500;
	int barW = 512, barH = 512;

	bool handEnable;  // ��Ŀ�� ��ȯ ����
	GLfloat handX;  // ��Ŀ�� x��ǥ, ���ٵ�� �� ���
	GLfloat handNum;  // �� Ŀ�� �ִϸ��̼ǿ� ���Ǵ� ��ġ
	bool fingerEnable; // �հ��� Ŀ�� ��ȯ ����

	// ������ ������
	bool exitEnable;
	GLfloat exitTransparent; // ������ ������ ����

	// info ������
	bool infoEnable;
	GLfloat infoTransparent;

	//��
	bool tipEnable;
	GLfloat tipTransparent;

	// Ÿ��Ʋ
	GLfloat titleY, titleTransparent, titleSize;

	// ���� ����
	bool intro;  // ��Ʈ�� �ִϸ��̼�

	bool menuEnable;  // �޴� Ȱ��ȭ ����
	bool menuOpened;  // �޴� ������ ��� ���� ����
	GLfloat menuY;  // �޴� ����
	GLfloat menuSizeX;  // �޴� ũ��
	GLfloat menuSizeY;
	GLfloat menuAcc;  // �޴��� �� �� ����ϴ� ���Ӱ�
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

			if (titleTransparent < 0)   // ������ ���������� �� �̻� �ִϸ��̼��� ������� �ʴ´�.
				intro = false;
		}
	}

    void exitGame() {  // esc�� ��� ���� ���� ����, ����â ������� ����
        if (exitEnable || infoEnable) {
            exitTransparent += fs / 6;  // ���� �������� ������ ��Ÿ���� ������ �����Ѵ�.
            if (exitTransparent > 1.0) {
                exitTransparent = 1.0;

				if (ExitOrInfo == 0)  // ����
					glutDestroyWindow(1);

				if (ExitOrInfo == 1) {  // ����â Ȱ��ȭ
					setInfo = true;
					infoEnable = false;
					exitTransparent = 0.0;

					// ����â �ʱ�ȭ
					info.infoTransparent = 1.0;
					info.size = 0.5;
					info.acc = 0.5;
				}
            }
        }

        else {
            exitTransparent -= fs / 6;  // ���� ���� �׸� �� ��� �������� �ٽ� ����������.
            if (exitTransparent < 0)
                exitTransparent = 0;
        }
    }

    void updateTip() {  // �� on/off
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
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

	void modelOutput(int idx) {  // �� ��� 
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

			if (handEnable)  // �� Ŀ��
				glBindTexture(GL_TEXTURE_2D, cursor[1]);
			else if (fingerEnable)  // �հ��� Ŀ��
				glBindTexture(GL_TEXTURE_2D, cursor[2]);
			else // �Ϲ� Ŀ��
				glBindTexture(GL_TEXTURE_2D, cursor[0]);

			if (!playFunc && cam.camRot == 0 && !cam.camR && !cam.camL)  // ī�޶� ȸ�� �� Ŀ���� ����
				glDrawArrays(GL_TRIANGLES, 0, 6);
			else if (playFunc)
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
	}

	void setObject(int idx) {  // ��ȯ ����
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

		case bar_:  // ȭ�� ũ�⿡ ���� �޴����� �ִ� ũ�Ⱑ �޶���
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
			if (lButtonDown && handEnable)  // ���ٵ��� ���� Ŀ���� ������ ������ ��ġ�� ��ȯ�Ѵ�.
				translateMatrix = translate(translateMatrix, vec3((handX - cam.camX), (0.3 - cam.camY), 0.0006));
			else
				translateMatrix = translate(translateMatrix, vec3((mx / cam.zoom - cam.camX) * ratio_, (my / cam.zoom - cam.camY), 1.0));
			break;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ
		transmit();

		glBindVertexArray(VAO_UI);  // �� �𵨸��� ������ VAO�� ���

		modelOutput(idx);
	}
};

extern UI ui;

#endif