// 키보드 조작
#include "gl_func.h"
#include "globalVar.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		ui.exitEnable = true;
		break;

	case 9:  // tab
		if (!ui.tipEnable) {
			ui.tipEnable = true;
			break;
		}
		else 
			ui.tipEnable = false;
		break;

	case 'q':  // 카메라 좌측 회전
		if (!alcy.touchEnable) {
			cam.camL = true;
			mouseClickEnable = false;
		}
		break;

	case 'e':  // 카메라 우측 회전
		if (!alcy.touchEnable) {
			cam.camR = true;
			mouseClickEnable = false;
		}
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void keyUp(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		ui.exitEnable = false;
		break;

	case 'q':  // 카메라 우측 회전 해제
		cam.camL = false;
		break;

	case 'e':  // 카메라 좌측 회전 해제
		cam.camR = false;
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
