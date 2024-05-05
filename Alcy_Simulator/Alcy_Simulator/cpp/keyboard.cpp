// 키보드 조작
#include "../header/gl_func.h"
#include "../header/Camera.h"


void key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		glutDestroyWindow(1);
		break;
	}

	// 홈 모드 카메라 조작
	if(fw.get_current_mode() == "home_mode")
		cam.key_down(KEY);
	
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}


void key_up(unsigned char KEY, int x, int y) {

	// 홈 모드 카메라 조작
	if (fw.get_current_mode() == "home_mode")
		cam.key_up(KEY);

	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
