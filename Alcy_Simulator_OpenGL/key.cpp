// 키보드 조작
#include "gl_func.h"
#include "gameVariable.h"

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		exitEnable = true;
		break;

	case 9:  // tab
		if (!tipEnable) {
			tipEnable = true;
			break;
		}
		else 
			tipEnable = false;
		break;

	case 'q':  // 카메라 좌측 회전
		if (!touchEnable) {
			camL = true;
			cursorEnable = false;
		}
		break;

	case 'e':  // 카메라 우측 회전
		if (!touchEnable) {
			camR = true;
			cursorEnable = false;
		}
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void keyUp(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		exitEnable = false;
		break;

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
