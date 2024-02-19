#include "gl_func.h"
#include "gameVariable.h"
#include "screen.h"

void convert_to_gl(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void setDir() {  // ��Ű �ٶ󺸴� ������ �����Ѵ�
	if (mx * ratio < -0.5 * ratio)
		dir = l;
	else if (mx * ratio > 0.5 * ratio)
		dir = r;
	else
		dir = m;
}

void updateCursor() {  // ��Ű �Ӹ��� Ŀ���� �����ٴ�� Ŀ���� �ٲ��
	if ((mx * ratio >= -0.4 && mx * ratio <= 0.4) && (my >= 0.1 && my <= 0.5))
		handEnable = true;
	else
		handEnable = false;
}

void Mouse(int button, int state, int x, int y) {  // ���콺 Ŭ��
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (handEnable && cursorEnable && camRot == 0) 
			touchEnable = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (handEnable && cursorEnable && touchEnable) {
			handNum = 0;
			headRot = 0;
			tailNum = 0;
			tailRot = 0;
			touchEnable = false;
		}
	}
	glutPostRedisplay();
}

void pMotion(int x, int y) {  // Ŭ�� ���� ���� ���
		convert_to_gl(x, y);
		camX = (0.0 - mx) / 10;
		camY = (0.0 - my) / 10;

		setDir();
		updateCursor();

	glutPostRedisplay();
}

void Motion(int x, int y) {  // Ŭ�� �� ���� ���
	if (!touchEnable) {
		convert_to_gl(x, y);
		camX = (0.0 - mx) / 10;
		camY = (0.0 - my) / 10;

		setDir();
		updateCursor();
	}

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // ���콺 ��
	if (dir > 0) {
		zoomAcc = 0.2 + (zoom - 1.0) / 10;
		zoomEnable = true;
	}
	else if (dir < 0) {
		if (zoom > 1.0) {
			zoomAcc = -0.2 - (zoom - 1.0) / 10;
			zoomEnable = true;
		}
	}
	
	return;
}