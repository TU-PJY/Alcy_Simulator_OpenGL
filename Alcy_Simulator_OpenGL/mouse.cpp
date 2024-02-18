#include "gl_func.h"
#include "screen.h"

// 커서 위치
extern GLfloat mx, my;

// 카메라
extern GLfloat ratio;
extern GLfloat camX, camY;
extern GLfloat camRot;

// 알키 바라보는 방향
extern int dir;

// 알키 머리 움직임
extern GLfloat headPos;

void convert_to_gl(int x, int y) {  //GL좌표계로 변환
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void setDir() {  // 알키 바라보는 방향을 결정한다
	if (mx * ratio < -0.5 * ratio)
		dir = l;
	else if (mx * ratio > 0.5 * ratio)
		dir = r;
	else
		dir = m;
}

void Mouse(int button, int state, int x, int y) {  // 마우스 클릭
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	glutPostRedisplay();
}

void pMotion(int x, int y) {  // 클릭 안할 때의 모션
	convert_to_gl(x, y);
	camX = (0.0 - mx) / 10;
	camY = (0.0 - my) / 10;

	setDir();

	glutPostRedisplay();
}

void Motion(int x, int y) {  // 클릭 할 때의 모션
	convert_to_gl(x, y);
	camX = (0.0 - mx) / 10;
	camY = (0.0 - my) / 10;

	setDir();

	glutPostRedisplay();
}