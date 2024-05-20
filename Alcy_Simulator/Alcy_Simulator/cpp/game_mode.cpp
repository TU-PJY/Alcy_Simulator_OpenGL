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
	cam.target_pos_y = -0.45;
	cam.zoom_value = 1.8;

	fw.AddMainObj(new GameboyBack(main_layer1, "gameboy_back"), main_layer1);

	fw.AddMainObj(new Gameboy(main_layer3, "gameboy"), main_layer3);

	fw.AddMainObj(new Front(main_layer3, "front_home"), main_layer3);


	glutMouseFunc(game_mode_mouse_button);
	glutMotionFunc(NULL);
	glutPassiveMotionFunc(NULL);
	glutMouseWheelFunc(NULL);
	glutKeyboardFunc(game_mode_key_down);
	glutKeyboardUpFunc(game_mode_key_up);
	glutSpecialFunc(game_mode_special_key_down);
	glutSpecialUpFunc(game_mode_special_key_up);
}


void game_mode_mouse_button(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	}


	// 메뉴 열기
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		glutWarpPointer(WIDTH / 2, HEIGHT / 2);
		fw.InitSubMode(return_menu_mode, "return_menu_mode", true);
	}
}

void game_mode_key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:	
		glutWarpPointer(WIDTH / 2, HEIGHT / 2);
		fw.InitSubMode(return_menu_mode, "return_menu_mode", true);
		break;
	}

	auto ptr = fw.FindMainObj_Layer_Single(main_layer3, "gameboy");
	if (ptr) ptr->gameboy_key_down(KEY, x, y);
}

void game_mode_key_up(unsigned char KEY, int x, int y) {
	auto ptr = fw.FindMainObj_Layer_Single(main_layer3, "gameboy");
	if (ptr) ptr->gameboy_key_up(KEY, x, y);
}

void game_mode_special_key_down(int KEY, int x, int y) {
	auto ptr = fw.FindMainObj_Layer_Single(main_layer3, "gameboy");
	if(ptr) ptr->gameboy_special_key_down(KEY, x, y);
}


void game_mode_special_key_up(int KEY, int x, int y) {
	auto ptr = fw.FindMainObj_Layer_Single(main_layer3, "gameboy");
	if(ptr) ptr->gameboy_special_key_up(KEY, x, y);
}