// 키보드 조작
#include "gl_func.h"

extern bool camR, camL;
extern GLfloat camRot;

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		glutDestroyWindow(1);
		break;

	case 'q':  // 카메라 좌측 회전
		camL = true;
		break;

	case 'e':  // 카메라 우측 회전
		camR = true;
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void keyUp(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 'q':  // 카메라 우측 회전 해제
		camL = false;
		break;

	case 'e':  // 카메라 좌측 회전 해제
		camR = false;
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
