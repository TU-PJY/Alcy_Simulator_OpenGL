#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"
#include "Icon.h"

class Menu : public SUB_CLS {
private:
	unsigned int tex;

	std::string tag{};
	int layer{};
	

	// 메뉴 배경 초기 위치
	GLfloat position = -1.0 -0.15;

	// 투명도
	GLfloat transparent{};

	// 아이콘
	Icon icon;

	// 버튼
	Button button;


public:
	// 메뉴 아이콘 개수 리턴
	size_t get_icon_number() const { return icon.get_icon_number(); }

	// 아이콘 영역 리턴
	std::array<GLfloat, 4> get_icon_zone(int i) const { return icon.get_icon_zone(i); }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor(int idx) { icon.tell_on_cursor(idx); }

	// 커서가 아이콘 위를 벗어낫음을 알림
	void tell_not_on_cursor(int idx) { icon.tell_not_on_cursor(idx); }

	// 커서가 아이콘을 눌렀음을 알림
	void tell_icon_click() { icon.tell_icon_click(); }


	// 버튼 관련 함수
	void tell_on_cursor_button(int idx) { button.tell_on_cursor(idx); }

	void tell_not_on_cursor_button(int idx) { button.tell_not_on_cursor(idx); }

	size_t get_button_number() { return button.get_button_number(); }

	void tell_button_click() { button.tell_button_click(); }

	std::array<GLfloat, 4> get_button_zone(int i) { return button.get_button_zone(i); }

	// info 관련 함수
	void tell_info_is_open() { button.tell_info_is_open(); }
	void tell_info_is_close() { button.tell_info_is_close(); }


	// 태그 리턴
	std::string GetTag() const { return tag; }


	void Update() {
		icon.tell_position(position);
		icon.Update();

		button.tell_position(position);
		button.Update();

		position = std::lerp(position, -1.0 + 0.15, fw.FT(10));
		transparent = std::lerp(transparent, 0.7, fw.FT(7));
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(rt(10.0), 1.5);
		set_object_static(0.0, dy(position));
		alpha = transparent;

		draw_image(tex);

		icon.Render();
		button.Render();
	}

	void CheckCollision() {}

	void CheckDelete() {}


	Menu(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//menu_back.png", 100, 100, 1);

		ssys_ui->playSound(menu_open, 0, false, &ch_ui);
	}

	~Menu() {
		ssys_ui->playSound(menu_close, 0, false, &ch_ui);
	}
};