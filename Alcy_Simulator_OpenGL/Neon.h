#ifndef NEON_H
#define NEON_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"


class Neon {
public:
	GLuint VAO_N;

	unsigned int nTex;
	int W = 1500, H = 1500;
	int channel;

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_N);
		glBindVertexArray(VAO_N);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &nTex);
		glBindTexture(GL_TEXTURE_2D, nTex);
		parameteri();
		texture_data = stbi_load("res//prop//neon.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void setObject() {
		using namespace glm;

		initTransform();
		scaleMatrix = scale(scaleMatrix, vec3(1.5, 1.5, 0.0));
		translateMatrix = translate(translateMatrix, vec3(-cam.camX / 2 + 0.7, -cam.camY / 2 + 0.4, -0.08));
		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환

		transmit();
		glBindVertexArray(VAO_N);

		glBindTexture(GL_TEXTURE_2D, nTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Neon neon;

#endif
