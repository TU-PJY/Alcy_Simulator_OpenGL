#ifndef START_H
#define START_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "gl_func.h"
#include "Sound.h"
#include "globalVar.h"
#include "main2.h"
#include "stb_image.h"

class Start {  // ���� ���� �� �ε� �� �ΰ� ��Ʈ�� ���
public:
	GLuint VAO_L;
	GLfloat backTransparent;  // �ε� ȭ�� ����
	GLfloat logoTransparent;  // �ΰ� ����
	GLfloat soundDelay;  // �ΰ� ���� ��� ���� �ð�
	GLfloat logoTime; // �ΰ� ��� �ð�
	GLfloat loadingDelay;  // �ε� ȭ�� ��ȯ ���� �ð�

	GLfloat size;  // �ΰ� ������
	GLfloat logoAcc; // �ΰ� ���� �ִϸ��̼� ���Ӱ�

	bool logoSoundPlayed; // �ΰ� ���� �ߺ� ��� ����
	bool loadingStart;  // �ΰ� ���� ��� �� �ε��� �����Ѵ�.
	bool themeSoundPlayed;  // ���� �׸� ���� �ߺ� ��� ����

	unsigned int logoTex[3];
	int W, H;
	int logoW, logoH;
	int channel;
	
	Start() {
		W = 512; H = 512;
		logoW = 500; logoH = 500;
		logoTransparent = 0.0;
		backTransparent = 1.0;
		size = 2.0;
	}

	void loading() {
		if (!loadingStart) {  // �ణ�� ������ �ڿ� �Ҹ��� ����ϰ� �� �ణ�� ������ �Ŀ� ���ҽ� �ε��� �����Ѵ�.
			if (!logoSoundPlayed) {
				soundDelay += fs;
				if (soundDelay > 10) {
					ssystem->playSound(logoSound, 0, false, &channelLogo);
					logoSoundPlayed = true;
					logoAcc = 0.5;
				}
			}

			if (logoSoundPlayed) { // �ΰ� ���尡 ��µʰ� ���ÿ� �ΰ� �ִϸ��̼��� ��µȴ�
				logoTransparent += logoAcc * fs;
				size -= logoAcc * fs / 1.5;
				logoAcc -= fs / 10;

				if (logoAcc < 0)
					logoAcc = 0;

				logoTime += fs;
				if (logoTime > 20)
					loadingStart = true;
			}
		}

		else if (loadingStart) {
			glutKeyboardFunc(keyDown);  // glut ��� Ȱ��ȭ
			glutKeyboardUpFunc(keyUp);
			glutMouseFunc(Mouse);
			glutMotionFunc(Motion);
			glutPassiveMotionFunc(pMotion);
			glutMouseWheelFunc(Wheel);

			initObject();  // ���ҽ� �ε�
			initFmod();
			stbi_image_free(texture_data);

			LoadingEnd = true;  // ���ҽ� �ε��� �Ϸ�Ǹ� ������ ���۵� �غ� �Ѵ�.
		}
	}

	void updateLoadingDisplay() {
		if(loadingDelay < 10)
			loadingDelay += fs;

		else if (loadingDelay > 10) { // �ε� �Ϸ� ���� �ε� ȭ���� ��ȯ�ϸ� ���� �ɸ��Ƿ� 1�� �� ���� ��ȯ��Ų��.
			logoTransparent -= fs / 10;
			backTransparent -= fs / 10;
			if (logoTransparent < 0.0)
				logoTransparent = 0.0;
			if (backTransparent < 0.0)
				backTransparent = 0.0;

			if (!themeSoundPlayed) {  // Ÿ��Ʋ ȭ�鿡 �����ϸ� ���� �׸����� ����Ѵ�.
				ssystem->playSound(mainTheme, 0, false, &channelTheme);
				themeSoundPlayed = true;
			}
		}
	}

	void setBuffer() {  // ���� ���� �ʱ�ȭ
		glGenVertexArrays(1, &VAO_L);
		glBindVertexArray(VAO_L);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &logoTex[0]);
		glBindTexture(GL_TEXTURE_2D, logoTex[0]);
		parameteri();
		texture_data = stbi_load("res//ui//black.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &logoTex[1]);
		glBindTexture(GL_TEXTURE_2D, logoTex[1]);
		parameteri();
		texture_data = stbi_load("res//ui//mata_logo.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, logoW, logoH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &logoTex[2]);
		glBindTexture(GL_TEXTURE_2D, logoTex[2]);
		parameteri();
		texture_data = stbi_load("res//ui//loading.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, logoW, logoH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void objectOut(int idx) {
		using namespace glm;

		initTransform();
		if (idx == 0) {  // back
			scaleMatrix = scale(scaleMatrix, vec3(10.0, 10.0, 0.0));
			translateMatrix = translate(translateMatrix, vec3(0.0, 0.0, 10.0));
			transparent = backTransparent;
		}
		else if (idx == 1) {  // logo
			scaleMatrix = scale(scaleMatrix, vec3(size, size, 0.0));
			translateMatrix = translate(translateMatrix, vec3(0.0, 0.0, 10.5));
			transparent = logoTransparent;
		}
		else if (idx == 2) {
			scaleMatrix = scale(scaleMatrix, vec3(0.5, 0.5, 0.0));
			translateMatrix = translate(translateMatrix, vec3(0.0, -0.5, 10.5));
			transparent = logoTransparent;
		}
		
		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ

		transmit();
		glBindVertexArray(VAO_L);

		glDepthMask(GL_FALSE);
		switch (idx) {
		case 0: glBindTexture(GL_TEXTURE_2D, logoTex[idx]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		case 1: 
			if (!logoSoundPlayed) break;
			glBindTexture(GL_TEXTURE_2D, logoTex[idx]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		case 2:
			if (!loadingStart) break;
			glBindTexture(GL_TEXTURE_2D, logoTex[idx]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
		glDepthMask(GL_TRUE);
	}
};

extern Start start;

#endif
