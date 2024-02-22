#include "gl_func.h"
#include "globalVar.h"
#include "screen.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"

void convertToGLCoord(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void setDir() {  // ��Ű �ٶ󺸴� ������ �����Ѵ�
		if (mx / cam.zoom * ratio < -0.5 * ratio)
			alcy.dir = l;
		else if (mx / cam.zoom * ratio > 0.5 * ratio)
			alcy.dir = r;
		else
			alcy.dir = m;
}

void updateCursor() {  // ��Ű �Ӹ��� Ŀ���� �����ٴ�� Ŀ���� �ٲ��
	if ((mx / cam.zoom * ratio >= -0.4 && mx / cam.zoom * ratio <= 0.4) && (my / cam.zoom >= 0.1 && my / cam.zoom <= 0.5)) // ���ٵ��
		ui.handEnable = true;
	else
		ui.handEnable = false;

	if ((mx / cam.zoom * ratio >= -0.03 && mx / cam.zoom * ratio <= 0.05) && (my / cam.zoom >= -0.2 && my / cam.zoom <= -0.15))  // �� ������
		ui.fingerEnable = true;
	else
		ui.fingerEnable = false;
}

void Mouse(int button, int state, int x, int y) {  // ���콺 Ŭ��
	int randomSound = 0;

	if (gameStarted) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if (ui.handEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak) {
				channelTouch->stop();
				ssystem->playSound(touch, 0, false, &channelTouch);   // ���ٵ�� ������ �������� �Ҹ��� �ݺ� ����Ѵ�.
				alcy.touchEnable = true;
				lButtonDown = true;
			}

			if (ui.fingerEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak) {
				random_device rd;  mt19937 gen(rd());
				uniform_int_distribution <int> dis(1, 3);
				randomSound = dis(gen);

				channelSqueak->stop();

				switch (randomSound) {  // 3���� �� �ϳ��� ����Ѵ�.
				case 1:
					ssystem->playSound(squeak1, 0, false, &channelSqueak);
					break;
				case 2:
					ssystem->playSound(squeak2, 0, false, &channelSqueak);
					break;
				case 3:
					ssystem->playSound(squeak3, 0, false, &channelSqueak);
					break;
				}

				alcy.squeakStartTime = time(NULL);
				alcy.squeak = true;
				lButtonDown = true;
			}
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			if (ui.handEnable && mouseClickEnable && alcy.touchEnable) {
				ui.handNum = 0;
				alcy.headRot = 0;
				alcy.tailNum = 0;
				alcy.tailRot = 0;
				alcy.bodyRot = 0;
				alcy.touchEnable = false;
				channelTouch->stop();  // ���ٵ�⸦ �ߴ��� ��� �Ҹ��� �����Ѵ�.
			}
			lButtonDown = false;
		}
	}
	glutPostRedisplay();
}

void pMotion(int x, int y) {  // Ŭ�� ���� ���� ���
		convertToGLCoord(x, y);
		if (gameStarted) {
			cam.camX = (0.0 - mx) / 10 / cam.zoom;
			cam.camY = (0.0 - my) / 10 / cam.zoom;

			if (!ui.menuEnable) {
				setDir();
				updateCursor();
			}

			alcy.isLeave = false;  // �����̸� ��Ʈ���� �����Ѵ�.
		}

	glutPostRedisplay();
}

void Motion(int x, int y) {  // Ŭ�� �� ���� ���
	if (gameStarted) {
		if (!alcy.touchEnable) {  // ��Ű�� ���ٵ�� ���߿��� ���콺 ������Ʈ�� ���� �ʴ´�.
			convertToGLCoord(x, y);
			cam.camX = (0.0 - mx) / 10 / cam.zoom;
			cam.camY = (0.0 - my) / 10 / cam.zoom;

			if (!ui.menuEnable) {
				setDir();
				updateCursor();
			}
		}

		alcy.isLeave = false;  // �����̸� ��Ʈ���� �����Ѵ�.
	}

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // ���콺 ��
	if (gameStarted) {
		if (dir > 0) {
			channelScroll->stop();
			ssystem->playSound(scroll, 0, false, &channelScroll);

			cam.zoomAcc = 0.25;
			cam.zoomEnable = true;

			alcy.isLeave = false;
		}
		else if (dir < 0) {
			channelScroll->stop();
			ssystem->playSound(scroll, 0, false, &channelScroll);

			if (cam.zoom > 1.0) {
				cam.zoomAcc = -0.25;
				cam.zoomEnable = true;
			}

			alcy.isLeave = false;
		}
	}
	
	return;
}