#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"

#include "ReturnIcon.h"

class ReturnMenu : public SUB_CLS {
private:
	unsigned int tex{};

	std::string tag{};
	int layer{};

	// 메뉴 배경 초기 위치
	GLfloat position = -1.0 - 0.15;

	// 투명도
	GLfloat transparent{};

	// 아이콘
	ReturnIcon icon;

public:
	// 아이콘 영역 리턴
	std::array<GLfloat, 4> get_icon_zone_return_menu() const { return icon.get_icon_zone(); }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor_return_menu() { icon.tell_on_cursor(); }

	// 커서가 아이콘 위를 벗어낫음을 알림
	void tell_not_on_cursor_return_menu() { icon.tell_not_on_cursor(); }

	// 커서가 아이콘을 눌렀음을 알림
	void tell_icon_click_return_menu() { icon.tell_icon_click(); }

	// 태그 리턴
	std::string GetTag() const { return tag; }


	void Update() {
		icon.tell_position(position);
		icon.Update();

		position = std::lerp(position, -1.0 + 0.15, fw.FT(10));
		transparent = std::lerp(transparent, 1.0, fw.FT(10));
	}


	void Render() {
		init_transform();

		s_mat *= scale_image(rt(10.0), 1.5);
		set_object_static(0.0, dy(position));
		alpha = transparent;

		draw_image(tex);

		icon.Render();
	}


	void CheckCollision() {}


	ReturnMenu(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//menu_back.png", 100, 100, 1);

		ssys_ui->playSound(menu_open, 0, false, &ch_ui);
	}


	~ReturnMenu() {
		ssys_ui->playSound(menu_close, 0, false, &ch_ui);

		glDeleteTextures(1, &tex);
	}
};