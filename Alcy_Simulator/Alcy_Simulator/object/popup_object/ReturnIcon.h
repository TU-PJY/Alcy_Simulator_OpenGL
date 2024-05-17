#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"
#include "../../header/mode_header.h"

class ReturnIcon {
private:
	unsigned int tex{};

	// 커서가 아이콘 위에 있는지 여부
	bool on_cursor{};
	bool on_click{};

	// 아이콘 커서 인식 영역
	std::array<GLfloat, 4> icon_zone{};

	// 아이콘 투명도
	GLfloat transparent{};

	// 아이콘 위치
	GLfloat position{};
	GLfloat position2{};


public:
	// 메뉴 배경의 위치를 아이콘에게 주기
	void tell_position (GLfloat pos) { position = pos; }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor() { on_cursor = true; }

	// 커서가 아이콘을 벗어났음을 알림
	void tell_not_on_cursor() { on_cursor = false; }

	// 아이콘 커서 인식 범위 리턴
	std::array<GLfloat, 4> get_icon_zone() const { return icon_zone; }


	// 마우스가 아이콘을 클릭했음을 알림
	void tell_icon_click() {
		if (on_cursor) {
			on_click = true;

			if (on_click) {
				// 현재 모드가 무엇이냐에 따라 돌아가는 모드가 달라진다
				if(fw.CurrentMainMode() == "game_mode")
					fw.SwitchMainMode(home_mode, "home_mode");
			}
		}
	}


	// 아이콘 커서 인식 범위 업데이트
	void update_icon_zone() {
		GLfloat x_min = rt(-1.0) + 0.05 + 0.01;
		GLfloat x_max = rt(-1.0) + 0.25 - 0.01;
		GLfloat y_min = position - 0.1 + 0.01;
		GLfloat y_max = position + 0.1 - 0.01;

		icon_zone = { dy(x_min), dy(x_max), dy(y_min), dy(y_max) };
	}


	void Update() {
		transparent = std::lerp(transparent, 1.0, fw.FT(7));

		// 커서를 마우스 위에 올리면 아이콘이 위로 올라오며 표시된다
		update_icon_zone();

		if (on_cursor)
			position2 = std::lerp(position2, 0.03, fw.FT(35));
		else
			position2 = std::lerp(position2, 0.0, fw.FT(35));
	}


	void Render() {
		init_transform();

		alpha = transparent;
		set_object_static(dy(rt(-1.0) + 0.15), dy(position + position2));

		draw_image(tex);
	}


	ReturnIcon() {
		set_texture(tex, "res//ui//menu//icon_return.png", 256, 256, 1);
	}
};

