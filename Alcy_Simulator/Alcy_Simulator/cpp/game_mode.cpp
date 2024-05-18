#include "../header/mode_header.h"
#include "../header/sound.h"
#include "../header/Camera.h"
#include "../header/gl_func.h"

#include "../object/main_object/Gameboy.h"
#include "../object/main_object/Front.h"

void game_mode() {
	cam.reset_camera();
	cam.camera_lock_state = true;
	cam.y = -1.0;

	fw.AddMainObj(new Gameboy(main_layer3, "gameboy"), main_layer3);
	fw.AddMainObj(new Front(main_layer3, "front_home"), main_layer3);


	glutMouseFunc(game_mode_mouse_button);
	glutMotionFunc(game_mode_mouse_motion);
	glutPassiveMotionFunc(game_mode_mouse_passive_motion);
	glutMouseWheelFunc(game_mode_wheel);
	glutKeyboardFunc(game_mode_key_down);
	glutKeyboardUpFunc(game_mode_key_up);
}


void game_mode_mouse_button(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	}


	// 메뉴 열기
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		fw.InitSubMode(return_menu_mode, "return_menu_mode");
	}
}


void game_mode_mouse_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void game_mode_mouse_passive_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void game_mode_wheel(int button, int dir, int x, int y) {
}


void game_mode_key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		fw.InitSubMode(return_menu_mode, "return_menu_mode");
		break;
	}
}


void game_mode_key_up(unsigned char KEY, int x, int y) {

}