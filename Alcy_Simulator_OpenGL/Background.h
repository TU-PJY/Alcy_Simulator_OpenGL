#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "stb_image.h"

class Background {
public:
	GLuint VAO_BACK;
	unsigned int backTex;
	int backgroundW, backgroundH;
	int channel;

	Background() {
		backgroundW = 2560, backgroundH = 1440;
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_BACK);
		glBindVertexArray(VAO_BACK);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}
	
	void setTexture() {
		glGenTextures(1, &backTex);
		glBindTexture(GL_TEXTURE_2D, backTex);
		parameteri();
		texture_data = stbi_load("res//ui//background.png", &backgroundW, &backgroundH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundW, backgroundH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void bindVertex() {
		glBindVertexArray(VAO_BACK);
	}

	void setTransform() {
		using namespace glm;
		scaleMatrix = scale(scaleMatrix, vec3(2.0 * ratio / cam.zoom, 2.0 / cam.zoom, 0.0));
		translateMatrix = translate(translateMatrix, vec3(cam.camX / 10 * ratio, (cam.camY / 10) / cam.zoom, -0.1));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
	}

	void modelOutput() {
		glBindTexture(GL_TEXTURE_2D, backTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Background background;

#endif
