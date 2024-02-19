#include "gl_func.h"
#include "screen.h"

// 커서
extern GLfloat mx, my;  // 커서 위치
extern bool handEnable;

// 카메라
extern GLfloat ratio;
extern GLfloat camX, camY;
extern GLfloat camRot;
extern GLfloat zoom, zoomAcc;
extern bool zoomEnable;

// 알키 바라보는 방향
extern int dir;

// 알키 관련 변수
extern GLfloat headPos;
extern GLfloat alcyHeight;

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

void updateCursor() {  // 알키 머리에 커서를 가져다대면 커서가 바뀐다
	if ((mx * ratio >= -0.4 && mx * ratio <= 0.4) && (my >= 0.1 && my <= 0.5))
		handEnable = true;
	else
		handEnable = false;
}

void Mouse(int button, int state, int x, int y) {  // 마우스 클릭
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	glutPostRedisplay();
}

void pMotion(int x, int y) {  // 클릭 안할 때의 모션
	convert_to_gl(x, y);
	camX = (0.0 - mx) / 10;
	camY = (0.0 + alcyHeight - my) / 10;

	setDir();
	updateCursor();

	glutPostRedisplay();
}

void Motion(int x, int y) {  // 클릭 할 때의 모션
	convert_to_gl(x, y);
	camX = (0.0 - mx) / 10;
	camY = (0.0 + alcyHeight - my) / 10;

	setDir();
	updateCursor();

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // 마우스 휠
	if (dir > 0) {
		zoomAcc = 0.2;
		zoomEnable = true;
	}
	else if (dir < 0) {
		if (zoom > 1.0) {
			zoomAcc = -0.2;
			zoomEnable = true;
		}
	}
	
	return;
}