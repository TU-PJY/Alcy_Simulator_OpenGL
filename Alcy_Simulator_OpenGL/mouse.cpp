#include "gl_func.h"
#include "screen.h"

extern GLfloat mx, my;
extern GLfloat camX, camY;

void convert_to_gl(int x, int y) {
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	glutPostRedisplay();
}

void Motion(int x, int y) {
	convert_to_gl(x, y);
	camX = (0.0 - mx) / 30;
	camY = (0.0 - my) / 30;

	glutPostRedisplay();
}