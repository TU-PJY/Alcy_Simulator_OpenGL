#ifndef GUITAR_H
#define GUITAR_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "stb_image.h"

class Guitar {
public:
	GLuint VAO_G;

	unsigned int gTex;
	int W = 1500, H = 1500;
	int channel;

	GLfloat guitarRot;
	GLfloat num;

	void update() {
		guitarRot = sin(num) * 1.5;
		num += fs / 2;
	}
	
	void setBuffer() {  // ���� ���� �ʱ�ȭ
		glGenVertexArrays(1, &VAO_G);
		glBindVertexArray(VAO_G);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &gTex);
		glBindTexture(GL_TEXTURE_2D, gTex);
		parameteri();
		texture_data = stbi_load("res//prop//guitar1.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void bindVertex() {
		glBindVertexArray(VAO_G);
	}

	void setTransform() {
		using namespace glm;
		translateMatrix = translate(translateMatrix, vec3(0.0, -0.75, -0.0001));
		translateMatrix = rotate(translateMatrix, radians(10 -guitarRot), vec3(0.0, 0.0, 1.0));
		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ
	}

	void modelOutput() {
		using namespace glm;

		glBindTexture(GL_TEXTURE_2D, gTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Guitar guitar;

#endif