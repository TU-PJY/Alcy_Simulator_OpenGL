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
	unsigned int lightTex[2];
	int W = 1500; int H = 1500;
	int channel;

	GLfloat lightTransparent;

	void updateLightTransparent() {  // 빛 투명도 업데이트
		if (beatDelay >= interval) 
			lightTransparent = 1.0;
		else {
			lightTransparent -= fs / 5;

			if (lightTransparent < 0.0)
				lightTransparent = 0.0;
		}
	}

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
		glGenTextures(1, &lightTex[0]);  // defalut light
		glBindTexture(GL_TEXTURE_2D, lightTex[0]);
		parameteri();
		texture_data = stbi_load("res//prop//light.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		glGenTextures(1, &lightTex[1]);  // house light
		glBindTexture(GL_TEXTURE_2D, lightTex[1]);
		parameteri();
		texture_data = stbi_load("res//prop//house_effect.png", &W, &H, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void objectOut() {
		using namespace glm;

		initTransform();

		if (funcNumber == 3) {
			scaleMatrix = scale(scaleMatrix, vec3(2.0, 3.0, 0.0));
			translateMatrix = translate(translateMatrix, vec3(0.0, 0.5, 0.05));
		}

		else if (funcNumber == 0) {  // 0번 기능 활성화일 때만 투명도를 적용한다.
			scaleMatrix = scale(scaleMatrix, vec3(3.0 / cam.zoom, 3.0 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(-cam.camX / 2 / cam.zoom, -cam.camY / 2 / cam.zoom, -0.08));
			transparent = lightTransparent;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환

		transmit();
		glBindVertexArray(VAO_L);

		glDepthMask(GL_FALSE);
		if(funcNumber == 3)
			glBindTexture(GL_TEXTURE_2D, lightTex[0]);
		else if(funcNumber == 0)
			glBindTexture(GL_TEXTURE_2D, lightTex[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDepthMask(GL_TRUE);
	}
};

extern Light light;


#endif
