// 키보드 조작
#include "gl_func.h"

//카메라
extern bool camR, camL;
extern GLfloat camRot;
extern GLfloat zoom;

// 커서
extern bool cursorEnable;

// 나가기 아이콘
extern bool exitEnable;

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		exitEnable = true;
		break;

	case 'q':  // 카메라 좌측 회전
		camL = true;
		cursorEnable = false;
		break;

	case 'e':  // 카메라 우측 회전
		camR = true;
		cursorEnable = false;
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
