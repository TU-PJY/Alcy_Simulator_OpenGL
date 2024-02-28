#ifndef INFO_H
#define INFO_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class Info {
public:
	GLuint VAO_I;

	unsigned int iTex;
	int W = 512, H = 512;
	int channel;

	GLfloat size;  // ũ��
	GLfloat acc;  // �ִϸ��̼� ���Ӱ�
	GLfloat infoTransparent;  // ����

	void update() {
		if (setInfo) {  // ����â�� Ŀ���� ��Ÿ����.
			size += acc * fs;
			acc -= fs / 5;
			if (acc < 0)
				acc = 0;
		}

		else if(!setInfo) {  // ����â�� ���������� �������.
			infoTransparent -= fs / 2;
			if (infoTransparent < 0.0)
				infoTransparent = 0.0;
		}
	}

	void setBuffer() {
		glGenVertexArrays(1, &VAO_I);
		glBindVertexArray(VAO_I);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &iTex);
		glBindTexture(GL_TEXTURE_2D, iTex);
		parameteri();
		texture_data = stbi_load("res//ui//info.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void objectOut() {
		using namespace glm;

		initTransform();
		scaleMatrix = scale(scaleMatrix, vec3(size / cam.zoom, size / cam.zoom, 0.0));
		translateMatrix = translate(translateMatrix, vec3((0.0 - cam.camX) * ratio_, 0.0 - cam.camY, 1.0));
		rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
		transparent = infoTransparent;

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ

		transmit();
		glBindVertexArray(VAO_I);

		if (size > 0.0) {
			glDepthMask(GL_FALSE);
			glBindTexture(GL_TEXTURE_2D, iTex);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDepthMask(GL_TRUE);
		}
		
	}
};

extern Info info;

#endif
