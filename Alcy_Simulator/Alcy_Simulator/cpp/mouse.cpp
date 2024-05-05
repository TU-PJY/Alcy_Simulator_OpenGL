#include "../header/gl_func.h"

GLfloat mx, my;

// ������ ��ǥ�� -> OpenGL ��ǥ�� ��ȯ
void convert_cursor_position(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}


void mouse_button(int button, int state, int x, int y) {  // ���콺 Ŭ��
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

	}

	glutPostRedisplay();
}


// Ŭ�� ���� ���� ���
void mouse_passive_motion(int x, int y) { 
	convert_cursor_position(x, y);

	glutPostRedisplay();
}


// Ŭ�� �� ���� ���
void mouse_motion(int x, int y) {  
	convert_cursor_position(x, y);

	glutPostRedisplay();
}


// ���콺 ��
void mouse_wheel(int button, int dir, int x, int y) { 
	if (dir > 0) {

	}

	else if (dir < 0) {

	}
}

