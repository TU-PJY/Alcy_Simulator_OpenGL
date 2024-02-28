#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class Background {
public:
	GLuint VAO_BACK;
	unsigned int backTex[8];
	int backgroundW, backgroundH;
	int channel;

	GLfloat idx;

	Background() {
		backgroundW = 1440, backgroundH = 1440;
	}

	void updateBackgroundIdx() {  // 배경 인덱스 업데이트
		idx += fs;
		if (idx > 3.99999)
			idx = 0;
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_BACK);
		glBindVertexArray(VAO_BACK);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}
	
	void setTexture() {
		glGenTextures(1, &backTex[0]);  // defalut
		glBindTexture(GL_TEXTURE_2D, backTex[0]);
		parameteri();
		texture_data = stbi_load("res//background//default.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &backTex[1]);  // house
		glBindTexture(GL_TEXTURE_2D, backTex[1]);
		parameteri();
		texture_data = stbi_load("res//background//house.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		// glitch 1 ~ 4
		glGenTextures(1, &backTex[2]);
		glBindTexture(GL_TEXTURE_2D, backTex[2]);
		parameteri();
		texture_data = stbi_load("res//background//glitch1.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &backTex[3]);
		glBindTexture(GL_TEXTURE_2D, backTex[3]);
		parameteri();
		texture_data = stbi_load("res//background//glitch2.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &backTex[4]);
		glBindTexture(GL_TEXTURE_2D, backTex[4]);
		parameteri();
		texture_data = stbi_load("res//background//glitch3.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &backTex[5]);
		glBindTexture(GL_TEXTURE_2D, backTex[5]);
		parameteri();
		texture_data = stbi_load("res//background//glitch4.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &backTex[6]);  // sunset
		glBindTexture(GL_TEXTURE_2D, backTex[6]);
		parameteri();
		texture_data = stbi_load("res//background//sunset.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &backTex[7]);  // wall
		glBindTexture(GL_TEXTURE_2D, backTex[7]);
		parameteri();
		texture_data = stbi_load("res//background//wall.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void objectOut() {
		using namespace glm;

		initTransform();

		scaleMatrix = scale(scaleMatrix, vec3(3.0 / cam.zoom, 3.0 / cam.zoom, 0.0));
		translateMatrix = translate(translateMatrix, vec3(-cam.camX / 2 / cam.zoom, (-cam.camY / 2) / cam.zoom, -0.1));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
		transmit();

		glBindVertexArray(VAO_BACK);
		if(playFunc && funcNumber == 0)
			glBindTexture(GL_TEXTURE_2D, backTex[1]);
		else if(playFunc && funcNumber == 1)
			glBindTexture(GL_TEXTURE_2D, backTex[(int)idx + 2]);
		else if (playFunc && funcNumber == 2)
			glBindTexture(GL_TEXTURE_2D, backTex[6]);
		else if (playFunc && funcNumber == 3)
			glBindTexture(GL_TEXTURE_2D, backTex[7]);
		else
			glBindTexture(GL_TEXTURE_2D, backTex[0]);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Background background;

#endif
