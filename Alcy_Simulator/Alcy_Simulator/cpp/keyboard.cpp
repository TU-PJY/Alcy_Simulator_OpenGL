// 키보드 조작
#include "../header/gl_func.h"
#include "../header/Camera.h"
#include "../header/mode_header.h"


void key_down(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		if (fw.get_current_mode() == "menu_mode")
			fw.end_popup();

		else {
			fw.init_popup(menu_mode, "menu_mode");
			cam.reset_key_state();  // 카메라 조작 상태를 초기화한다 

			auto ptr = fw.find_object(layer7, "cursor_home");
			if (ptr != nullptr)
				ptr->reset_mouse_state();
		}

		break;
	}

	// 홈 모드 카메라 조작
	if (fw.get_current_mode() == "home_mode") {
		cam.key_down(KEY);

		auto ptr = fw.find_object(layer3, "tip_home");
		if (ptr != nullptr)
			ptr->update_tip_visible(KEY);
	}
}


void key_up(unsigned char KEY, int x, int y) {

	// 홈 모드 카메라 조작
	if (fw.get_current_mode() == "home_mode")
		cam.key_up(KEY);
}
