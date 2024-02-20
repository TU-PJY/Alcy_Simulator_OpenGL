#ifndef UI_H
#define UI_H
#include "config.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "translate.h"
#include "gameVariable.h"
//#include "Alcy.h"
//#include "Camera.h"

class Alcy;
class Cam;

class UI {
public:
	// Ŀ��
	bool cursorEnable;  // Ŀ�� ���� ����
	bool handEnable;  // ��Ŀ�� ��ȯ ����
	GLfloat handX;  // ��Ŀ�� x��ǥ, ���ٵ�� �� ���
	GLfloat handNum;  // �� Ŀ�� �ִϸ��̼ǿ� ���Ǵ� ��ġ

	// ������ ������
	bool exitEnable;
	GLfloat exitTransparent; // ������ ������ ����

	//��
	bool tipEnable;
	GLfloat tipTransparent;

	UI() {
		cursorEnable = true;
		tipEnable = true;
		tipTransparent = 1.0f;
	}
    // ui
    void exitGame() {  // esc�� ��� ���� ���� ����
        if (exitEnable) {
            exitTransparent += fs / 6;
            if (exitTransparent > 1.0) {
                exitTransparent = 1.0;
                glutDestroyWindow(1);
            }
        }
        else {
            exitTransparent -= fs / 6;
            if (exitTransparent < 0) {
                exitTransparent = 0;
            }
        }
    }

    void updateTip() {  // �� on/off
        if (tipEnable) {
            tipTransparent += fs / 3;
            if (tipTransparent > 1.0)
                tipTransparent = 1.0;
        }
        else {
            tipTransparent -= fs / 3;
            if (tipTransparent < 0.0)
                tipTransparent = 0.0;
        }
    }

	void finishTransform(int idx) {  // ��ȯ ���� 
		colorLocation = glGetUniformLocation(ID, "targetColor");
		glUniform3f(colorLocation, selectedColor.r, selectedColor.g, selectedColor.b);

		thresholdLocation = glGetUniformLocation(ID, "colorThreshold");
		glUniform3f(thresholdLocation, threshold.r, threshold.g, threshold.b);

		transparencyLocation = glGetUniformLocation(ID, "transparency");
		glUniform1f(transparencyLocation, transparent);

		projectionLocation = glGetUniformLocation(ID, "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

		viewLocation = glGetUniformLocation(ID, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		objColorLocation = glGetUniformLocation(ID, "objectColor");
		glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

		viewPosLocation = glGetUniformLocation(ID, "viewPos"); // viewPos �� ����: ī�޶� ��ġ
		glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

		modelLocation = glGetUniformLocation(ID, "model"); // ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(transformMatrix)); // modelTransform ������ ��ȯ �� �����ϱ�

		glBindVertexArray(VAO_UI[idx]);  // �� �𵨸��� ������ VAO�� ���
	}

	void setTransform(int idx) {  // ��ȯ ����
		using namespace glm;
		transformMatrix = mat4(1.0f);  // ���� ���

		scaleMatrix = mat4(1.0f);  // ���� ���
		rotateMatrix = mat4(1.0f);  // ȸ�� ���
		translateMatrix = mat4(1.0f);  // �̵� ���

		selectedColor = vec3(0.0, 0.0, 0.0);  // Ŭ���� ��� ����
		threshold = vec3(0.0, 0.0, 0.0);  // Ŭ���� �Ӱ� ����
		transparent = 1.0f;

		switch (idx) {  // ��ȯ �߰� 
		case 0:
			scaleMatrix = scale(scaleMatrix, vec3(2.0 * ratio / cam.zoom, 2.0 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3(-cam.camX * ratio / 4, -cam.camY / 4, -0.001));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.85, 0.0);
			break;

		case 1:  // ��
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 0.0));
			translateMatrix = translate(translateMatrix, vec3((-0.7 - cam.camX) * ratio, 0.5 - cam.camY, 0.0005));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.8, 0.0);
			transparent = tipTransparent;
			break;

		case 2:  // ������ ǥ��
			scaleMatrix = scale(scaleMatrix, vec3(0.5 / cam.zoom, 0.5 / cam.zoom, 1.0));
			translateMatrix = translate(translateMatrix, vec3((0.0 - cam.camX) * ratio, 0.0 - cam.camY, 0.0005));
			rotateMatrix = rotate(rotateMatrix, radians(-cam.camRot), vec3(0.0, 0.0, 1.0));

			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.8, 0.0);
			transparent = exitTransparent;
			break;

		case 3:  // cursor, �׻� �� �������� ���
			scaleMatrix = scale(scaleMatrix, vec3(0.1 / cam.zoom, 0.1 / cam.zoom, 1.0));
			if (!alcy.touchEnable)
				translateMatrix = translate(translateMatrix, vec3((mx - cam.camX) * ratio, my - cam.camY, 0.001));
			else if (alcy.touchEnable)
				translateMatrix = translate(translateMatrix, vec3((handX - cam.camX) * ratio, 0.3 - cam.camY, 0.001));
			selectedColor = vec3(0.0, 1.0, 0.0);
			threshold = vec3(0.0, 0.85, 0.0);
			break;
		}

		transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ
	}

	void modelOutput(int idx) {  // �� ��� 
		switch (idx) {
		case 0:
			glBindTexture(GL_TEXTURE_2D, back);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case 1:  // tip
			glBindTexture(GL_TEXTURE_2D, tip);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case 2:  // icon
			glBindTexture(GL_TEXTURE_2D, icon[0]);  // exit button
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;

		case 3:  // cursor
			if (!handEnable)
				glBindTexture(GL_TEXTURE_2D, cursor[0]);
			else
				glBindTexture(GL_TEXTURE_2D, cursor[1]);
			if (cam.camRot == 0 && !cam.camR && !cam.camL)  // ī�޶� ȸ�� �� Ŀ���� ����
				glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
	}

};

extern UI ui;

#endif