// 키보드 조작
#include "../header/gl_func.h"
#include "../header/Camera.h"
#include "../header/mode_header.h"


void key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		glutDestroyWindow(1);
		break;
	
	//  메뉴 열고 닫기
	case 32:
		if (fw.get_current_mode() == "menu_mode")
			fw.end_popup();

		else {
			fw.init_popup(menu_mode, "menu_mode");
			cam.reset_key_state();  // 카메라 조작 상태를 초기화한다 

			auto ptr = fw.get_ptr(cursor_layer, 0); // 마우스 조작 상태를 초기화 한다
			if (ptr != nullptr) 
				ptr->reset_mouse_state();
		}
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
