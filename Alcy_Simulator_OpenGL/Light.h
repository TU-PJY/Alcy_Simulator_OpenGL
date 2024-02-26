#ifndef LIGHT_H
#define LIGHT_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class Light {
public:
	GLuint VAO_L;
	unsigned int lightTex;
	int W = 1500; int H = 1500;
	int channel;

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
		glGenTextures(1, &lightTex);
		glBindTexture(GL_TEXTURE_2D, lightTex);
		parameteri();
		texture_data = stbi_load("res//prop//light.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void setObject() {
		using namespace glm;

		initTransform();

		scaleMatrix = scale(scaleMatrix, vec3(2.0, 3.0, 0.0));
		translateMatrix = translate(translateMatrix, vec3(0.0, 0.5, 0.05));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ

		transmit();
		glBindVertexArray(VAO_L);

		glBindTexture(GL_TEXTURE_2D, lightTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Light light;


#endif
