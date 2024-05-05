#ifndef NOTE_H
#define NOTE_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class Note {
public:
	GLuint VAO_N;

	unsigned int nTex;
	int W = 200, H = 200;
	int channel;

	GLfloat noteTransparent;
	GLfloat rot;
	GLfloat x, y;
	GLfloat size;
	GLfloat num;
	GLfloat num2;

	Note() {
		x = 0.4; y = 0.4;
	}

	void update() {  // ��ǥ ������Ʈ �ִϸ��̼� ������Ʈ
		rot = sin(num) * 10;
		num += fs;

		noteTransparent = sin(num2);
		num2 += fs / 10;
		size = noteTransparent / 5;
		x += fs / 50;
		y += fs / 50;

		if (noteTransparent < 0) {
			num2 = 0;
			x = 0.4;
			y = 0.4;
		}
	}

	void setBuffer() {  // ���� ���� �ʱ�ȭ
		glGenVertexArrays(1, &VAO_N);
		glBindVertexArray(VAO_N);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &nTex);
		glBindTexture(GL_TEXTURE_2D, nTex);
		parameteri();
		texture_data = stbi_load("res//prop//note.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void objectOut() {
		using namespace glm;

		initTransform();
		scaleMatrix = scale(scaleMatrix, vec3(size, size, 0.0));
		translateMatrix = translate(translateMatrix, vec3(x, y, 0.0005));
		translateMatrix = rotate(translateMatrix, radians(rot), vec3(0.0, 0.0, 1.0));
		transparent = noteTransparent;

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ

		transmit();
		glBindVertexArray(VAO_N);

		glBindTexture(GL_TEXTURE_2D, nTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Note note;

#endif
