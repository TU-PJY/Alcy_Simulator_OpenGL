#include "../header/mode_header.h"

#include "../object/popup_object/ReturnMenu.h"
#include "../object/popup_object/Cursor_ReturnMenu.h"
#include "../object/popup_object/Back.h"

#include "../header/gl_func.h"


void set_func_to_game_mode();
void set_func_to_return_menu_mode();
void close_return_menu();


void return_menu_mode() {
	fw.AddSubObj(new Back(sub_layer1, "back_return_menu"), sub_layer1);
	fw.AddSubObj(new ReturnMenu(sub_layer1, "return_menu"), sub_layer1);
	fw.AddSubObj(new CursorReturnMenu(sub_layer4, "cursor_return_menu"), sub_layer4);

	set_func_to_return_menu_mode();

}


void return_menu_mode_mouse_button(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		auto ptr = fw.FindSubObj_Layer_Single(sub_layer4, "cursor_return_menu");
		if (ptr) ptr->mouse_left_button_down(button, state);
	}

	// ¸Þ´º ´Ý±â
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		close_return_menu();
	}
}


void return_menu_mode_mouse_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void return_menu_mode_mouse_passive_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void return_menu_mode_wheel(int button, int dir, int x, int y) {}


void return_menu_mode_key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		close_return_menu();
		break;
	}
}


void return_menu_mode_key_up(unsigned char KEY, int x, int y) {}


void close_return_menu() {
	if (fw.CurrentMainMode() == "game_mode")
		set_func_to_game_mode();

	fw.EndSubMode();
}


void set_func_to_game_mode() {
	glutMouseFunc(game_mode_mouse_button);
	glutMotionFunc(NULL);
	glutPassiveMotionFunc(NULL);
	glutMouseWheelFunc(game_mode_wheel);
	glutKeyboardFunc(game_mode_key_down);
	glutKeyboardUpFunc(game_mode_key_up);
}


void set_func_to_return_menu_mode() {
	glutMouseFunc(return_menu_mode_mouse_button);
	glutMotionFunc(return_menu_mode_mouse_motion);
	glutPassiveMotionFunc(return_menu_mode_mouse_passive_motion);
	glutMouseWheelFunc(return_menu_mode_wheel);
	glutKeyboardFunc(return_menu_mode_key_down);
	glutKeyboardUpFunc(return_menu_mode_key_up);
}