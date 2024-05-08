#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"
#include "Info.h"

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
	std::array<bool, ICON_NUMBER> on_click{};

	// 아이콘 커서 인식 영역
	std::array<GLfloat, 4> icon_zone{};

	// 아이콘 투명도
	GLfloat transparent{};

	// 아이콘 위치
	GLfloat position{};
	GLfloat position2[ICON_NUMBER]{};


public:
	// 메뉴 배경의 위치를 아이콘에게 주기
	void tell_position(GLfloat pos) { position = pos; }

	// 아이콘 개수 리턴
	size_t get_icon_number() const { return ICON_NUMBER; }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor(int idx) { on_cursor[idx] = true; }

	// 커서가 아이콘을 벗어났음을 알림
	void tell_not_on_cursor(int idx) { on_cursor[idx] = false; }

	// 마우스가 아이콘을 클릭했음을 알림
	void tell_icon_click() { 
		for (int i = 0; i < ICON_NUMBER; ++i) {
			if (on_cursor[i]) {
				on_click[i] = true;
				ssys_ui->playSound(menu_click, 0, false, &ch_ui);

				
				on_click[i] = false;
			}
		}
	}

	// 아이콘 커서 인식 범위 리턴
	std::array<GLfloat, 4> get_icon_zone() const { return icon_zone; }
	

	// 아이콘 커서 인식 범위 업데이트
	void update_icon_zone() {
		GLfloat x_min = -1.0 * ratio + 0.05 + 0.01;
		GLfloat x_max = -1.0 * ratio + 0.25 - 0.01;
		GLfloat y_min = position - 0.1 + 0.01;
		GLfloat y_max = position + 0.1 - 0.01;

		icon_zone = { x_min, x_max, y_min, y_max };
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
		for (int i = 0; i < ICON_NUMBER; ++i) {
			init_transform();

			alpha = transparent;
			set_object_static((- 1.0 * ratio + 0.15 + 0.25 * i) / cam.zoom, (position + position2[i]) / cam.zoom);

			draw_image(tex[i], VAO);
		}
	}



	Icon() {
		set_canvas(VAO);

		for (int i = 0; i < ICON_NUMBER; ++i)
			set_texture(tex[i], directory[i], 256, 256, 1);
	}
};



class Button {
private:
	GLuint VAO;
	std::array<unsigned int, 2> tex{};
	std::array<const char*, 2> directory = {
		"res//ui//icon_off.png",
		"res//ui//icon_info.png",
	};

	std::array<GLfloat, 4> button_zone{};


	std::array<bool, 2> on_cursor{};
	std::array<bool, 2> on_click{};


	// 아이콘 투명도
	GLfloat transparent{};

	// 아이콘 위치
	GLfloat position{};
	GLfloat position2[2]{};

	bool info_is_open{};


public:
	// 메뉴 배경의 위치를 아이콘에게 주기
	void tell_position(GLfloat pos) { position = pos; }

	// 아이콘 개수 리턴
	size_t get_button_number() const { return tex.size(); }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor(int idx) { on_cursor[idx] = true; }

	// 커서가 아이콘을 벗어났음을 알림
	void tell_not_on_cursor(int idx) { on_cursor[idx] = false; }

	void tell_info_is_open() { info_is_open = true; }
	void tell_info_is_close() { info_is_open = false; }

	// 마우스가 아이콘을 클릭했음을 알림
	void tell_button_click() {
		for (int i = 0; i < tex.size(); ++i) {
			if (on_cursor[i]) {
				on_click[i] = true;
				ssys_ui->playSound(menu_click, 0, false, &ch_ui);

				switch (i) {
				case 0:  // exit game
					glutDestroyWindow(1);
					break;
				
				case 1: // info
					if(!info_is_open)
						fw.add_popup_object(new Info(popup_layer2, "info"), popup_layer2);
					break;
				}

				on_click[i] = false;
			}
		}
	}

	// 아이콘 커서 인식 범위 리턴
	std::array<GLfloat, 4> get_button_zone() const { return button_zone; }


	// 아이콘 커서 인식 범위 업데이트
	void update_icon_zone() {
		GLfloat x_min = -1.0 * ratio + 0.1 - 0.08 + 0.01;
		GLfloat x_max = -1.0 * ratio + 0.1 + 0.08 - 0.01;
		GLfloat y_min = position + 0.25 - 0.08 + 0.01;
		GLfloat y_max = position + 0.25 + 0.08 - 0.01;

		button_zone = { x_min, x_max, y_min, y_max };
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

			s_mat *= scale_image(0.8, 0.8);

			alpha = transparent;
			set_object_static((-1.0 * ratio + 0.1 + position2[i]) / cam.zoom, (position + 0.25 + 0.16 * i) / cam.zoom);

			draw_image(tex[i], VAO);
		}
	}


	Button() {
		set_canvas(VAO);
		for (int i = 0; i < tex.size(); ++i)
			set_texture(tex[i], directory[i], 512, 512, 1);
	}

};