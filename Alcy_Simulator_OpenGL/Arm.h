#ifndef ARM_H
#define ARM_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "stb_image.h"

class Arm {
public:
	GLuint VAO_A;

	unsigned int aTex;
	int W = 1500, H = 1500;
	int channel;

	GLfloat armRot;
	GLfloat num;

	void update() {
		armRot = sin(num) * 5;
		num += fs;
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_A);
		glBindVertexArray(VAO_A);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &aTex);
		glBindTexture(GL_TEXTURE_2D, aTex);
		parameteri();
		texture_data = stbi_load("res//prop//arm.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void bindVertex() {
		glBindVertexArray(VAO_A);
	}

	void setTransform() {
		using namespace glm;
		translateMatrix = translate(translateMatrix, vec3(-0.395, -0.6, 0.0));
		translateMatrix = rotate(translateMatrix, radians(armRot), vec3(0.0, 0.0, 1.0));
		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
	}

	void modelOutput() {
		using namespace glm;

		glBindTexture(GL_TEXTURE_2D, aTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Arm arm;

#endif
