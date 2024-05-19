#include "../header/mode_header.h"

#include "../object/popup_object/HomeMenu.h"
#include "../object/popup_object/Cursor_HomeMenu.h"
#include "../object/popup_object/Back.h"

#include "../header/gl_func.h"


void set_func_to_home_mode();

void menu_mode() {
	fw.AddSubObj(new Back(sub_layer1, "back"), sub_layer1);
	fw.AddSubObj(new HomeMenu(sub_layer1, "menu"), sub_layer1);
	fw.AddSubObj(new CursorHomeMenu(sub_layer4, "cursor_menu"), sub_layer4);

	glutMouseFunc(menu_mode_mouse_button);
	glutMotionFunc(menu_mode_mouse_motion);
	glutPassiveMotionFunc(menu_mode_mouse_passive_motion);
	glutMouseWheelFunc(menu_mode_wheel);
	glutKeyboardFunc(menu_mode_key_down);
	glutKeyboardUpFunc(menu_mode_key_up);
	glutSpecialFunc(NULL);
	glutSpecialUpFunc(NULL);
}


void menu_mode_mouse_button(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		auto ptr = fw.FindSubObj_Layer_Single(sub_layer4, "cursor_menu");
		if (ptr != nullptr)
			ptr->mouse_left_button_down(button, state);
	}

	// ¸Þ´º ´Ý±â
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		set_func_to_home_mode();

		fw.EndSubMode();
	}
}


void menu_mode_mouse_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void menu_mode_mouse_passive_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void menu_mode_wheel(int button, int dir, int x, int y) {

}


void menu_mode_key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		set_func_to_home_mode();
		
		fw.EndSubMode();
		break;
	}
}


void menu_mode_key_up(unsigned char KEY, int x, int y) {

}

void set_func_to_home_mode() {
	glutMouseFunc(home_mode_mouse_button);
	glutMotionFunc(home_mode_mouse_motion);
	glutPassiveMotionFunc(home_mode_mouse_passive_motion);
	glutMouseWheelFunc(home_mode_wheel);
	glutKeyboardFunc(home_mode_key_down);
	glutKeyboardUpFunc(home_mode_key_up);
	glutSpecialFunc(NULL);
	glutSpecialUpFunc(NULL);
}