#include "../header/mode_header.h"

#include "../object/main_object/Alcy_home.h"
#include "../object/main_object/Cursor_home.h"
#include "../object/main_object/BackGround_home.h"
#include "../object/main_object/Shadow.h"
#include "../object/main_object/Front.h"
#include "../object/main_object/Tip_home.h"
#include "../object/main_object/Zoom_ind.h"

#include "../header/sound.h"
#include "../header/Camera.h"
#include "../header/gl_func.h"


void home_mode() {
	fw.AddMainObj(new BackGround_home(main_layer1, "background_home"), main_layer1);

	fw.AddMainObj(new Shadow(main_layer1, "shadow_home"), main_layer1);

	fw.AddMainObj(new Alcy_home(main_layer2, "alcy_home"), main_layer2);

	fw.AddMainObj(new Tip_home(main_layer3, "tip_home"), main_layer3);

	fw.AddMainObj(new Zoom_ind(main_layer3, "zoom_ind"), main_layer3);

	fw.AddMainObj(new Front(main_layer3, "front_home"), main_layer3);

	fw.AddMainObj(new Cursor_home(main_layer7, "cursor_home"), main_layer7);

	ssys->playSound(home_music, 0, false, &ch_bgm);

	cam.reset_camera();
	cam.zoom = 2.4;
	cam.zoom_value = 0.8;
	cam.zoom_count = 3;

	glutMouseFunc(home_mode_mouse_button);
	glutMotionFunc(home_mode_mouse_motion);
	glutPassiveMotionFunc(home_mode_mouse_passive_motion);
	glutMouseWheelFunc(home_mode_wheel);
	glutKeyboardFunc(home_mode_key_down);
	glutKeyboardUpFunc(home_mode_key_up);
}


void home_mode_mouse_button(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		auto ptr = fw.FindMainObj_Layer_Single(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->mouse_left_button_down(button, state);
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		auto ptr = fw.FindMainObj_Layer_Single(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->mouse_left_button_up(button, state);
	}


	// 메뉴 열기
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		fw.InitSubMode(menu_mode, "menu_mode");
		cam.reset_key_state();  // 카메라 조작 상태를 초기화한다 

		auto ptr = fw.FindMainObj_Layer_Single(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->reset_mouse_state();
	}
}


void home_mode_mouse_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void home_mode_mouse_passive_motion(int x, int y) {
	convert_cursor_position(x, y);
}


void home_mode_wheel(int button, int dir, int x, int y) {
	cam.scroll(dir);
}


void home_mode_key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		fw.InitSubMode(menu_mode, "menu_mode");
		cam.reset_key_state();  // 카메라 조작 상태를 초기화한다 

		auto ptr = fw.FindMainObj_Layer_Single(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->reset_mouse_state();
		break;
	}

	// 홈 모드 카메라 조작
	cam.key_down(KEY);

	// 팁 on/off
	auto ptr = fw.FindMainObj_Layer_Single(main_layer3, "tip_home");
	if (ptr != nullptr)
		ptr->update_tip_visible(KEY);
}


void home_mode_key_up(unsigned char KEY, int x, int y) {
	cam.key_up(KEY);
}