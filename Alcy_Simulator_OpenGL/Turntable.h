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

class Turntable {
public:
	GLuint VAO_TT;

	unsigned int thTex[5];
	int W, H;
	int channel;
	GLfloat ttBeatEffect;
	GLfloat idx;

	Turntable() {
		int W = 1500; int H = 1500;
	}

	void updateTurntableIndex() {
		idx += fs;
		if (idx > 3.99999)
			idx = 0;
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_TT);
		glBindVertexArray(VAO_TT);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

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
		texture_data = stbi_load("res//prop//turntable_glitch_1.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &thTex[2]);
		glBindTexture(GL_TEXTURE_2D, thTex[2]);
		parameteri();
		texture_data = stbi_load("res//prop//turntable_glitch_2.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &thTex[3]);
		glBindTexture(GL_TEXTURE_2D, thTex[3]);
		parameteri();
		texture_data = stbi_load("res//prop//turntable_glitch_3.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &thTex[4]);
		glBindTexture(GL_TEXTURE_2D, thTex[4]);
		parameteri();
		texture_data = stbi_load("res//prop//turntable_glitch_4.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void setObject() {
		using namespace glm;
		initTransform();

		scaleMatrix = scale(scaleMatrix, vec3(1.5, 1.5, 0.0));
		translateMatrix = translate(translateMatrix, vec3(0.0, -0.65, 0.0009));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
		transmit();

		glBindVertexArray(VAO_TT);

		if (funcNumber == 0)  // 선택한 아이콘에 따라 턴테이블 형태가 달라진다.
			glBindTexture(GL_TEXTURE_2D, thTex[0]);
		else if (funcNumber == 1)
			glBindTexture(GL_TEXTURE_2D, thTex[(int)idx + 1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Turntable turntable;

#endif
