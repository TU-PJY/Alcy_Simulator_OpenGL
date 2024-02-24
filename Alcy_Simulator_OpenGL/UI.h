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

    void exitGame() {  // esc�� ��� ���� ���� ����
        if (exitEnable) {
            exitTransparent += fs / 6;  // ���� �������� ������ ��Ÿ���� ������ �����Ѵ�.
            if (exitTransparent > 1.0) {
                exitTransparent = 1.0;
                glutDestroyWindow(1);
            }
        }
        else {
            exitTransparent -= fs / 6;  // ���� ���� �׸� �� ��� �������� �ٽ� ����������.
            if (exitTransparent < 0) {
                exitTransparent = 0;
            }
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

	void bindVertex(int idx) {
		glBindVertexArray(VAO_UI[idx]);  // �� �𵨸��� ������ VAO�� ���
	}

	void setTransform(int idx) {  // ��ȯ ����
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

		case bar_:  // ȭ�� ũ�⿡ ���� �޴����� �ִ� ũ�Ⱑ �޶���
			scaleMatrix = scale(scaleMatrix, vec3((0.2 + menuSizeX) * ratio_ / cam.zoom, (0.01 + menuSizeY) / cam.zoom, 0.0));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			translateMatrix = translate(translateMatrix, vec3(-cam.camX * ratio_, (menuY + menuSizeY / 1.2) / cam.zoom - cam.camY, 0.1));
			transparent = menuTransparent;
			break;

		case cursor_:
			scaleMatrix = scale(scaleMatrix, vec3(0.15 / cam.zoom, 0.15 / cam.zoom, .0));

			if (lButtonDown && handEnable)  // ���ٵ��� ���� Ŀ���� ������ ������ ��ġ�� ��ȯ�Ѵ�.
				translateMatrix = translate(translateMatrix, vec3((handX - cam.camX), (0.3 - cam.camY), 0.0006));
			else
				translateMatrix = translate(translateMatrix, vec3((mx / cam.zoom - cam.camX) * ratio_, (my / cam.zoom - cam.camY), 1.0));
			break;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ
	}

	void modelOutput(int idx) {  // �� ��� 
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
			if (handEnable)  // �� Ŀ��
				glBindTexture(GL_TEXTURE_2D, cursor[1]);
			else if (fingerEnable)  // �հ��� Ŀ��
				glBindTexture(GL_TEXTURE_2D, cursor[2]);
			else // �Ϲ� Ŀ��
				glBindTexture(GL_TEXTURE_2D, cursor[0]);

			if (cam.camRot == 0 && !cam.camR && !cam.camL && gameStarted)  // ī�޶� ȸ�� �� Ŀ���� ����
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
	}
};

extern UI ui;

#endif