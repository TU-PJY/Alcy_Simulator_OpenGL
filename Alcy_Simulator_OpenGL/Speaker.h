#ifndef SPEAKRER_H
#define SPEAKER_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "stb_image.h"

class Speaker {
public:
	GLuint VAO_SP;

	unsigned int spTex[2];
	int W, H;
	int channel;
	int count;
	int dir;

	Speaker() {
		int W = 1500; int H = 1500;
	}

	void updateSpeakerBeat() {
		if (beatDelay >= interval)
			count++;
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_SP);
		glBindVertexArray(VAO_SP);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &spTex[0]);
		glBindTexture(GL_TEXTURE_2D, spTex[0]);
		parameteri();
		texture_data = stbi_load("res//prop//speaker_1.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &spTex[1]);
		glBindTexture(GL_TEXTURE_2D, spTex[1]);
		parameteri();
		texture_data = stbi_load("res//prop//speaker_2.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void bindVertex() {
		glBindVertexArray(VAO_SP);
	}

	void setTransform() {
		using namespace glm;
		scaleMatrix = scale(scaleMatrix, vec3(3.0, 3.0, 0.0));
		translateMatrix = translate(translateMatrix, vec3(0.0, -0.3, -0.05));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
	}

	void modelOutput() {
		using namespace glm;
		if(count % 2 == 0)
			glBindTexture(GL_TEXTURE_2D, spTex[0]);
		else if(count % 2 == 1)
			glBindTexture(GL_TEXTURE_2D, spTex[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Speaker speaker;

#endif
