#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"
#include "Icon.h"

class Menu : public POPUP_FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex;

	std::string tag{};
	int layer{};
	

	// 메뉴 배경 초기 위치
	GLfloat position = -1.0 -0.15;

	// 투명도
	GLfloat transparent{};

	// 아이콘
	Icon icon;


public:
	// 메뉴 아이콘 개수 리턴
	size_t get_icon_number() const { return icon.get_icon_number(); }

	// 아이콘 영역 리턴
	std::array<GLfloat, 4> get_icon_zone() const { return icon.get_icon_zone(); }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor(int idx) { icon.tell_on_cursor(idx); }

	// 커서가 아이콘 위를 벗어낫음을 알림
	void tell_not_on_cursor(int idx) { icon.tell_not_on_cursor(idx); }


	void update() {
		icon.tell_position(position);
		icon.update();

		position = std::lerp(position, -1.0 + 0.15, fw.calc_ft(10));
		transparent = std::lerp(transparent, 0.7, fw.calc_ft(7));
	}

	void render() {
		init_transform();
		s_mat *= scale_image(10.0 * ratio, 1.5);
		set_object_static(0.0, position / cam.zoom);
		alpha = transparent;

		draw_image(tex, VAO);

		icon.render();
	}

	void check_collision() {}

	void check_delete() {}


	Menu(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//ui//menu_back.png", 100, 100, 1);

		ssys_ui->playSound(menu_open, 0, false, &ch_ui);
	}

	~Menu() {
		ssys_ui->playSound(menu_close, 0, false, &ch_ui);
	}
};