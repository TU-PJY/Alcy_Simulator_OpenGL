#ifndef WHITE_H
#define WHITE_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class White {
public:
	GLuint VAO_WHITE;

	unsigned int whiteTex; // 0: �Ϲ� ������, 1: ���� ���� ������
	int W, H;
	int channel;

	White() {
		W = 512; H = 512;
	}

	void update() {
		if (whiteTransparent > 0.0)
			whiteTransparent -= fs / 2;

		if (whiteTransparent < 0.0)
			whiteTransparent = 0.0;
	}

	void setBuffer() {  // ���� ���� �ʱ�ȭ
		glGenVertexArrays(1, &VAO_WHITE);
		glBindVertexArray(VAO_WHITE);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &whiteTex);
		glBindTexture(GL_TEXTURE_2D, whiteTex);
		parameteri();
		texture_data = stbi_load("res//ui//white.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void setObject() {
		using namespace glm;

		initTransform();

		scaleMatrix = scale(scaleMatrix, vec3(10.0 * ratio_, 10.0, 0.0));
		translateMatrix = translate(translateMatrix, vec3(0.0, 0.0, 1.0));
		transparent = whiteTransparent;

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ

		transmit();
		glBindVertexArray(VAO_WHITE);

		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, whiteTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDepthMask(GL_TRUE);
	}
};

extern White white;

#endif