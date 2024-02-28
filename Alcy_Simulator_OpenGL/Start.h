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

class Start {  // 게임 시작 시 로딩 및 로고 인트로 담당
public:
	GLuint VAO_L;
	GLfloat backTransparent;  // 로딩 화면 투명도
	GLfloat logoTransparent;  // 로고 투명도
	GLfloat soundDelay;  // 로고 사운드 출력 지연 시간
	GLfloat logoTime; // 로고 출력 시간
	GLfloat loadingDelay;  // 로딩 화면 전환 지연 시간

	GLfloat size;  // 로고 사이즈
	GLfloat logoAcc; // 로고 등장 애니메이션 가속값

	bool logoSoundPlayed; // 로고 사운드 중복 재생 방지
	bool loadingStart;  // 로고 사운드 출력 후 로딩을 시작한다.
	bool themeSoundPlayed;  // 메인 테마 사운드 중복 재생 방지

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
		if (!loadingStart) {  // 약간의 딜레이 뒤에 소리를 출력하고 또 약간의 딜레이 후에 리소스 로딩을 시작한다.
			if (!logoSoundPlayed) {
				soundDelay += fs;
				if (soundDelay > 10) {
					ssystem->playSound(logoSound, 0, false, &channelLogo);
					logoSoundPlayed = true;
					logoAcc = 0.5;
				}
			}

			if (logoSoundPlayed) { // 로고 사운드가 출력됨과 동시에 로고 애니메이션이 출력된다
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
			glutKeyboardFunc(keyDown);  // glut 기능 활성화
			glutKeyboardUpFunc(keyUp);
			glutMouseFunc(Mouse);
			glutMotionFunc(Motion);
			glutPassiveMotionFunc(pMotion);
			glutMouseWheelFunc(Wheel);

			initObject();  // 리소스 로딩
			initFmod();
			stbi_image_free(texture_data);

			LoadingEnd = true;  // 리소스 로딩이 완료되면 게임이 시작될 준비를 한다.
		}
	}

	void updateLoadingDisplay() {
		if(loadingDelay < 10)
			loadingDelay += fs;

		else if (loadingDelay > 10) { // 로딩 완료 직후 로딩 화면을 전환하면 렉이 걸리므로 1초 후 지연 전환시킨다.
			logoTransparent -= fs / 10;
			backTransparent -= fs / 10;
			if (logoTransparent < 0.0)
				logoTransparent = 0.0;
			if (backTransparent < 0.0)
				backTransparent = 0.0;

			if (!themeSoundPlayed) {  // 타이틀 화면에 진입하면 메인 테마곡을 재생한다.
				ssystem->playSound(mainTheme, 0, false, &channelTheme);
				themeSoundPlayed = true;
			}
		}
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_L);
		glBindVertexArray(VAO_L);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
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
		
		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환

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
