#ifndef SPEAKRER_H
#define SPEAKER_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"

class Speaker {
public:
	GLuint VAO_SP;

	unsigned int spTex[6];
	int W, H;
	int channel;
	int count;
	int dir;

	GLfloat imageIdx;

	Speaker() {
		int W = 1500; int H = 1500;
	}

	void updateSpeakerBeat() {  // ����Ŀ ���� ȿ�� ������Ʈ
		if (beatDelay >= interval)
			count++;
	}

	void updateImageIndex() {  // ����Ŀ �̹��� �ε��� ������Ʈ
		imageIdx += fs;
		if (imageIdx > 3.99999)
			imageIdx = 0;
	}

	void setBuffer() {  // ���� ���� �ʱ�ȭ
		glGenVertexArrays(1, &VAO_SP);
		glBindVertexArray(VAO_SP);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
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

		glGenTextures(1, &spTex[2]);
		glBindTexture(GL_TEXTURE_2D, spTex[2]);
		parameteri();
		texture_data = stbi_load("res//prop//speaker_glitch_1.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &spTex[3]);
		glBindTexture(GL_TEXTURE_2D, spTex[3]);
		parameteri();
		texture_data = stbi_load("res//prop//speaker_glitch_2.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &spTex[4]);
		glBindTexture(GL_TEXTURE_2D, spTex[4]);
		parameteri();
		texture_data = stbi_load("res//prop//speaker_glitch_3.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &spTex[5]);
		glBindTexture(GL_TEXTURE_2D, spTex[5]);
		parameteri();
		texture_data = stbi_load("res//prop//speaker_glitch_4.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void objectOut() {
		using namespace glm;

		initTransform();

		scaleMatrix = scale(scaleMatrix, vec3(3.0, 3.0, 0.0));
		translateMatrix = translate(translateMatrix, vec3(-cam.camX / 2, -0.3 - cam.camY / 2, -0.05));

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ

		transmit();
		glBindVertexArray(VAO_SP);


		if (funcNumber == 0) {
			if (count % 2 == 0)
				glBindTexture(GL_TEXTURE_2D, spTex[0]);
			else if (count % 2 == 1)
				glBindTexture(GL_TEXTURE_2D, spTex[1]);
		}
		else if (funcNumber == 1)
			glBindTexture(GL_TEXTURE_2D, spTex[(int)imageIdx + 2]);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

extern Speaker speaker;

#endif
