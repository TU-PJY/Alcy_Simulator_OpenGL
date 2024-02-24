#ifndef TURNTABLE_H
#define TURNTABLE_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "stb_image.h"

class Turntable {
public:
	GLuint VAO_TT;

	unsigned int thTex[2];
	int W, H;
	int channel;
	GLfloat ttBeatEffect;

	Turntable() {
		int W = 1500; int H = 1500;
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_TT);
		glBindVertexArray(VAO_TT);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &thTex[0]);
		glBindTexture(GL_TEXTURE_2D, thTex[0]);
		parameteri();
		texture_data = stbi_load("res//prop//turntable_house.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &thTex[1]);
		glBindTexture(GL_TEXTURE_2D, thTex[1]);
		parameteri();
		texture_data = stbi_load("res//prop//turntable_house.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void bindVertex() {
		glBindVertexArray(VAO_TT);
	}

	void setTransform() {
		using namespace glm;
		translateMatrix = translate(translateMatrix, vec3(0.0, -0.75, 0.0009));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
	}

	void modelOutput() {
		using namespace glm;
		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, thTex[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDepthMask(GL_TRUE);
	}
};

extern Turntable turntable;

#endif
