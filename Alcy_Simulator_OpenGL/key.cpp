// 키보드 조작
#include "gl_func.h"

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		glutDestroyWindow(1);
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void keyUp(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 0:
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
