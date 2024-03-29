#ifndef ZZZ_H
#define ZZZ_H
#include "config.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "Alcy.h"

class ZZZ {
public:
	GLuint VAO_ZZZ;
	GLfloat x, y;
	GLfloat size;
	GLfloat zzzTransparent;
	GLfloat num;
	GLfloat delayData; // 출력 딜레이 저장
	GLfloat delay;  // 출력 딜레이
	unsigned int zzzTex;
	int zzzW = 200, zzzH = 200;
	//unsigned char* texture_data;
	int channel;

	ZZZ() {
		x = 0.4, y = 0.4;
	}

	void update() {  // 애니메이션 업데이트
		if (alcy.sleeping) {  // 알키가 잠을 자면 업데이트 한다.
			if (delay >= 0) {
				delay -= fs;
			}

			if (delay < 0) {
				zzzTransparent = sin(num);
				num += fs / 10;
				size = zzzTransparent / 5;
				x += fs / 50;
				y += fs / 50;

				if (zzzTransparent < 0) {
					num = 0;
					x = 0.4;
					y = 0.4;
				}
			}
		}

		else {  // 아니라면 초기화 후 하지 않는다.
			zzzTransparent -= fs / 10;  // 중간에 알키를 깨우면 서서히 사라진다.
			if (zzzTransparent < 0) {
				zzzTransparent = 0;
				num = 0;
				x = 0.4;
				y = 0.4;
				delay = delayData;  // 딜레이를 입력받았던 딜레이로 초기화
			}
		}
	}

	void setDelay(int idx) {  // 출력 딜레이 설정
		delay = 10 * idx;
		delayData = 10 * idx;
	}

	void setBuffer() {  // 프롭 버퍼 초기화
		glGenVertexArrays(1, &VAO_ZZZ);
		glBindVertexArray(VAO_ZZZ);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
		glEnableVertexAttribArray(2);
	}

	void setTexture() {
		glGenTextures(1, &zzzTex);
		glBindTexture(GL_TEXTURE_2D, zzzTex);
		parameteri();
		texture_data = stbi_load("res//prop//zzz.png", &zzzW, &zzzH, &channel, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, zzzW, zzzH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void objectOut() {
		using namespace glm;

		initTransform();

		scaleMatrix = scale(scaleMatrix, vec3(size, size, 0.0));
		translateMatrix = translate(translateMatrix, vec3(x, y, 0.0005));
		transparent = zzzTransparent;

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환

		transmit();

		glBindVertexArray(VAO_ZZZ);

		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, zzzTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDepthMask(GL_TRUE);
	}
};

extern ZZZ zzz[3];

#endif
