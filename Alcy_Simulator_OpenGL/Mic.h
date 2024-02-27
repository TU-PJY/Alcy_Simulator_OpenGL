#ifndef MIC_H
#define MIC_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class Mic {
public:
	GLuint VAO_M;

	unsigned int mTex;
	int W = 1500, H = 1500;
	int channel;

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_M);
		glBindVertexArray(VAO_M);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &mTex);
		glBindTexture(GL_TEXTURE_2D, mTex);
		parameteri();
		texture_data = stbi_load("res//prop//mic.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void setObject() {
		using namespace glm;

		initTransform();
		scaleMatrix = scale(scaleMatrix, vec3(1.5, 1.5, 0.0));
		translateMatrix = translate(translateMatrix, vec3(cam.camX / 2 + 1.5, cam.camY / 2 - 0.5, 0.06));
		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환

		transmit();
		glBindVertexArray(VAO_M);

		glBindTexture(GL_TEXTURE_2D, mTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Mic mic;

#endif
