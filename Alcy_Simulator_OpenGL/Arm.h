#ifndef ARM_H
#define ARM_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class Arm {
public:
	GLuint VAO_A;

	unsigned int aTex[2];
	int W = 1500, H = 1500;
	int channel;

	GLfloat armRot;
	GLfloat num;

	void update() {
		if (funcNumber == 2) {
			armRot = sin(num) * 5;
			num += fs;
		}
		else if (funcNumber == 3) {
			armRot = sin(num) * 10;
			num += fs * 2;
		}
	}

	void setBuffer() {  // ���� ���� �ʱ�ȭ
		glGenVertexArrays(1, &VAO_A);
		glBindVertexArray(VAO_A);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &aTex[0]);
		glBindTexture(GL_TEXTURE_2D, aTex[0]);
		parameteri();
		texture_data = stbi_load("res//prop//arm_guitar1.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &aTex[1]);
		glBindTexture(GL_TEXTURE_2D, aTex[1]);
		parameteri();
		texture_data = stbi_load("res//prop//arm_guitar2.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void setObject() {
		using namespace glm;

		initTransform();

		translateMatrix = translate(translateMatrix, vec3(-0.395, -0.6, 0.0));
		translateMatrix = rotate(translateMatrix, radians(armRot), vec3(0.0, 0.0, 1.0));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ

		transmit();
		glBindVertexArray(VAO_A);

		if (funcNumber == 2)
			glBindTexture(GL_TEXTURE_2D, aTex[0]);
		else if (funcNumber == 3)
			glBindTexture(GL_TEXTURE_2D, aTex[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Arm arm;

#endif
