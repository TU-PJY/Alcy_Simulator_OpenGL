#include "../header/gl_func.h"
#include "../header/Camera.h"
#include "../header/mode_header.h"

GLfloat mx, my;

// ������ ��ǥ�� -> OpenGL ��ǥ�� ��ȯ
void convert_cursor_position(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}


void mouse_button(int button, int state, int x, int y) {  // ���콺 Ŭ��

	// Ȩ ��� ���콺 Ŭ��
	if (fw.CurrentMode() == "home_mode") {
		auto ptr = fw.FindMainObj(main_layer7, "cursor_home");

		if (ptr != nullptr) {
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
				ptr->mouse_left_button_down(button, state);

			else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
				ptr->mouse_left_button_up(button, state);
		}
	}


	// �޴� ��� ���콺 Ŭ��
	if (fw.CurrentMode() == "menu_mode") {
		auto ptr = fw.FindSubObj(sub_layer4, "cursor_menu");

		if (ptr != nullptr) {
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
				ptr->mouse_left_button_down(button, state);
		}
	}


	// �޴� ���� / �ݱ�
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (fw.CurrentMode() == "menu_mode")
			fw.EndSubMode();

		else {
			fw.InitSubMode(menu_mode, "menu_mode");
			cam.reset_key_state();  // ī�޶� ���� ���¸� �ʱ�ȭ�Ѵ� 

			auto ptr = fw.FindMainObj(main_layer7, "cursor_home");
			if (ptr != nullptr)
				ptr->reset_mouse_state();
		}
	}
}


// Ŭ�� ���� ���� ���
void mouse_passive_motion(int x, int y) { 
	convert_cursor_position(x, y);
}


// Ŭ�� �� ���� ���
void mouse_motion(int x, int y) {  
	convert_cursor_position(x, y);
}


// ���콺 ��
void mouse_wheel(int button, int dir, int x, int y) { 
	if (fw.CurrentMode() == "home_mode")
		cam.scroll(dir);
}

