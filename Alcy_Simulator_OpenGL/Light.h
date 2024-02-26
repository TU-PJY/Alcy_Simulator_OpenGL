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

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_L);
		glBindVertexArray(VAO_L);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
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

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환

		transmit();
		glBindVertexArray(VAO_L);

		glBindTexture(GL_TEXTURE_2D, lightTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Light light;


#endif
