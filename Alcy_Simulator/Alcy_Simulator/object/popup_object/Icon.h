#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"

// 아이콘 개수
#define ICON_NUMBER 4

class Icon {
private:
	GLuint VAO{};
	std::array<unsigned int, ICON_NUMBER> tex{};
	std::array<const char*, ICON_NUMBER> directory = {
		"res//ui//menu//icon1.png",
		"res//ui//menu//icon2.png",
		"res//ui//menu//icon3.png",
		"res//ui//menu//icon4.png",
	};

	// 커서가 아이콘 위에 있는지 여부
	std::array<bool, ICON_NUMBER> on_cursor{};

	// 아이콘 커서 인식 영역
	std::array<GLfloat, ICON_NUMBER> icon_zone{};

	// 아이콘 투명도
	GLfloat transparent{};

	// 아이콘 위치
	GLfloat position{};
	GLfloat position2[ICON_NUMBER]{};


public:
	// 메뉴 배경의 위치를 아이콘에게 주기
	void tell_position(GLfloat pos) { position = pos; }

	// 아이콘 개수 리턴
	size_t get_icon_number() const { return tex.size(); }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor(int idx) { on_cursor[idx] = true; }
	void tell_not_on_cursor(int idx) { on_cursor[idx] = false; }

	// 아이콘 커서 인식 범위 리턴
	std::array<GLfloat, 4> get_icon_zone() const { return icon_zone; }
	

	// 아이콘 커서 인식 범위 업데이트
	void update_icon_zone() {
		GLfloat x_min = -1.0 * ratio + 0.05 + 0.01;
		GLfloat x_max = -1.0 * ratio + 0.25 - 0.01;
		GLfloat y_min = position - 0.1 + 0.01;
		GLfloat y_max = position + 0.1 - 0.01;

		icon_zone = {
			x_min, x_max, y_min, y_max
		};
	}


	void update() {
		update_icon_zone();

		transparent = std::lerp(transparent, 1.0, fw.calc_ft(7));

		// 커서를 마우스 위에 올리면 아이콘이 위로 올라오며 표시된다
		for (int i = 0; i < tex.size(); ++i) {
			if (on_cursor[i])
				position2[i] = std::lerp(position2[i], 0.03, fw.calc_ft(35));
			else
				position2[i] = std::lerp(position2[i], 0.0, fw.calc_ft(35));
		}
	}

	void render() {
		for (int i = 0; i < tex.size(); ++i) {
			init_transform();

			alpha = transparent;
			set_object_static((- 1.0 * ratio + 0.15 + 0.25 * i) / cam.zoom, (position + position2[i]) / cam.zoom);
			draw_image(tex[i], VAO);
		}
	}

	Icon() {

		set_canvas(VAO);

		for (int i = 0; i < tex.size(); ++i)
			set_texture(tex[i], directory[i], 256, 256, 1);
	}
};