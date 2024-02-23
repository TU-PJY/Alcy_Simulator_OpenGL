#ifndef ICON_H
#define ICON_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "Camera.h"
#include "Alcy.h"
#include "Ui.h"
#include "stb_image.h"

class Icon {
public:
	GLuint VAO_ICON;
	GLfloat iconTransparent;
	GLfloat Xpos;

	unsigned int iconTex;
	int iconW, iconH;
	int channel;

	bool isOnCursor;  // true�� �� �������� ���õ� ��

	Icon() {
		iconTransparent = 0.0;
		iconW = 512, iconH = 512;
	}

	void updateIcon() {  // �޴��� �� �� ����ū�� �ε巴�� ��Ÿ����.
		if (ui.menuEnable && ui.menuTransparent < 0.8) {
			iconTransparent += fs / 2;
			if (iconTransparent > 1.0)
				iconTransparent = 1.0;
		}

		else if (!ui.menuEnable) {  // �޴��� ���� ���� ���� ������ �������.
			iconTransparent -= fs;
			if (iconTransparent < 0)
				iconTransparent = 0;
		}
	}

	void updateOnCursor() {  // Ŀ�� ���� ������Ʈ
		if (isOnCursor && ui.menuOpened) {  // Ŀ���� �����ܿ� �ø��� ���� ����������
			iconTransparent -= fs;
			if (iconTransparent < 0.5)
				iconTransparent = 0.5;
		}
	}

	void setBuffer() {  // ���� ���� �ʱ�ȭ
		glGenVertexArrays(1, &VAO_ICON);
		glBindVertexArray(VAO_ICON);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		vertexInput(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
		glEnableVertexAttribArray(2);
	}

	void setTexture(int idx) {
		glGenTextures(1, &iconTex);
		glBindTexture(GL_TEXTURE_2D, iconTex);
		parameteri();
		switch (idx) {  // �� ��ü���� �ٸ� �������� �ε���
			case 0: texture_data = stbi_load("res//ui//menu//icon_drum_base.png", &iconW, &iconH, &channel, 4);  // icon_drun
				break;
			case 1: texture_data = stbi_load("res//ui//menu//icon_electronic.png", &iconW, &iconH, &channel, 4);  // icon_electronic
				break;
			case 2: texture_data = stbi_load("res//ui//menu//icon_guitar1.png", &iconW, &iconH, &channel, 4);  // icon_guitar1
				break;
			case 3: texture_data = stbi_load("res//ui//menu//icon_guitar2.png", &iconW, &iconH, &channel, 4);  // icon_guitar2
				break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iconW, iconH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	}

	void bindVertex() {
		glBindVertexArray(VAO_ICON);
	}

	void setTransform(int idx) {
		using namespace glm;
		scaleMatrix = scale(scaleMatrix, vec3(0.2 / cam.zoom, 0.2 / cam.zoom, 0.0));
		translateMatrix = translate(translateMatrix, vec3(-cam.camX * ratio + (-1.0 * ratio + 0.15 * ratio + idx * 0.35) / cam.zoom, -cam.camY - 0.33 / cam.zoom, 0.002));
		rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
		transparent = iconTransparent;

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ
	}

	void modelOutput() {
		using namespace glm;
		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, iconTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDepthMask(GL_TRUE);
	}
};

extern Icon icon[ICON_PART];


#endif