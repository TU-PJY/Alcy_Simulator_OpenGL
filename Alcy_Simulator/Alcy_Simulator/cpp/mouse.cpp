#include "../header/gl_func.h"
#include "../header/Camera.h"
#include "../header/mode_header.h"

GLfloat mx, my;

// 윈도우 좌표계 -> OpenGL 좌표계 변환
void convert_cursor_position(int x, int y) {  //GL좌표계로 변환
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}


void mouse_button(int button, int state, int x, int y) {  // 마우스 클릭

	// 홈 모드 마우스 클릭
	if (fw.get_current_mode() == "home_mode") {
		auto ptr = fw.get_ptr(cursor_layer, 0);

		if (ptr != nullptr) {
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
				ptr->mouse_left_button_down(button, state);

			else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
				ptr->mouse_left_button_up(button, state);
		}
	}


	// 메뉴 모드 마우스 클릭
	if (fw.get_current_mode() == "menu_mode") {
		auto ptr = fw.get_popup_ptr(popup_cursor_layer, 0);
		if (ptr != nullptr) {
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
				ptr->mouse_left_button_down(button, state);
		}
	}


	// 메뉴 열기 / 닫기
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (fw.get_current_mode() == "menu_mode")
			fw.end_popup();

		else {
			fw.init_popup(menu_mode, "menu_mode");
			cam.reset_key_state();  // 카메라 조작 상태를 초기화한다 

			auto ptr = fw.get_ptr(cursor_layer, 0); // 마우스 조작 상태를 초기화 한다
			if (ptr != nullptr)
				ptr->reset_mouse_state();
		}
	}
}


// 클릭 안할 때의 모션
void mouse_passive_motion(int x, int y) { 
	convert_cursor_position(x, y);
}


// 클릭 할 때의 모션
void mouse_motion(int x, int y) {  
	convert_cursor_position(x, y);
}


// 마우스 휠
void mouse_wheel(int button, int dir, int x, int y) { 
	if (fw.get_current_mode() == "home_mode")
		cam.scroll(dir);
}

