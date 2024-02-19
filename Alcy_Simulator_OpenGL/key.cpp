// 키보드 조작
#include "gl_func.h"

//카메라
extern bool camR, camL;
extern GLfloat camRot;
extern GLfloat zoom;

//일시정지
extern bool pause;
extern GLfloat pauseAcc;

// 커서
extern bool cursorEnable;

extern GLfloat alcyHeight;

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		//glutDestroyWindow(1);
		if (!pause) {
			alcyHeight = 0.0;
			pause = true;
			pauseAcc = 0.3;
			camRot = 0;  // 카메라가 회전해있다면 모두 초기화한다.
			camR = false;
			camL = false;
			zoom = 1.0;  // 줌도 초기화 한다.
			break;
		}
		if (pause) {
			alcyHeight = 0.46682;
			pause = false;
			pauseAcc = 0.3;
			camRot = 0;  // 카메라가 회전해있다면 모두 초기화한다.
			camR = false;
			camL = false;
			zoom = 1.0;  // 줌도 초기화 한다.
		}
		break;

	case 'q':  // 카메라 좌측 회전
		if (!pause && alcyHeight == 0) {
			camL = true;
			cursorEnable = false;
		}
		break;

	case 'e':  // 카메라 우측 회전
		if (!pause && alcyHeight == 0) {
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
