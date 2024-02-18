#include "gl_func.h"
#include "screen.h"

// Ŀ�� ��ġ
extern GLfloat mx, my;

// ī�޶�
extern GLfloat ratio;
extern GLfloat camX, camY;
extern GLfloat camRot;

// ��Ű �ٶ󺸴� ����
extern int dir;

// ��Ű �Ӹ� ������
extern GLfloat headPos;

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

void Mouse(int button, int state, int x, int y) {  // ���콺 Ŭ��
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	glutPostRedisplay();
}

void pMotion(int x, int y) {  // Ŭ�� ���� ���� ���
	convert_to_gl(x, y);
	camX = (0.0 - mx) / 10;
	camY = (0.0 - my) / 10;

	setDir();

	glutPostRedisplay();
}

void Motion(int x, int y) {  // Ŭ�� �� ���� ���
	convert_to_gl(x, y);
	camX = (0.0 - mx) / 10;
	camY = (0.0 - my) / 10;

	setDir();

	glutPostRedisplay();
}