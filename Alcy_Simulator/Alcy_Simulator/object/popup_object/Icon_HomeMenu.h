#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"
#include "../../header/mode_header.h"
#include "Info.h"

// 아이콘 개수
#define ICON_NUMBER 5

class IconHomeMenu {
private:
	std::array<unsigned int, ICON_NUMBER> tex{};
	std::array<unsigned int, ICON_NUMBER> tex_stop{};

	std::array<const char*, ICON_NUMBER> directory = {
		"res//ui//menu//icon5.png",
		"res//ui//menu//icon1.png",
		"res//ui//menu//icon2.png",
		"res//ui//menu//icon3.png",
		"res//ui//menu//icon4.png",
	};


	// 커서가 아이콘 위에 있는지 여부
	std::array<bool, ICON_NUMBER> on_cursor{};
	std::array<bool, ICON_NUMBER> on_click{};

	// 아이콘 커서 인식 영역
	std::array< std::array<GLfloat, 4>, ICON_NUMBER> icon_zone{};

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

	// 아이콘 커서 인식 범위 리턴
	std::array<GLfloat, 4> get_icon_zone(int i) const { return icon_zone[i]; }

	// 마우스가 아이콘을 클릭했음을 알림
	void tell_icon_click() { 
		bool click{};

		for (int i = 0; i < ICON_NUMBER; ++i) {
			if (on_cursor[i]) {
				on_click[i] = true;

				switch (i) {
				case 0:
					ch_bgm->stop();
					fw.SwitchMainMode(game_mode, "game_mode");
					break;
				}
				
				break;
			}
		}
	}

	

	// 아이콘 커서 인식 범위 업데이트
	void update_icon_zone(int i) {
		GLfloat x_min = rt(-1.0) + 0.05 + 0.01 + 0.25 * i;
		GLfloat x_max = rt(-1.0) + 0.25 - 0.01 + 0.25 * i;
		GLfloat y_min = position - 0.1 + 0.01;
		GLfloat y_max = position + 0.1 - 0.01;

		icon_zone[i] = {dy(x_min), dy(x_max), dy(y_min), dy(y_max)};
	}


	void Update() {
		transparent = std::lerp(transparent, 1.0, fw.FT(7));

		// 커서를 마우스 위에 올리면 아이콘이 위로 올라오며 표시된다
		for (int i = 0; i < ICON_NUMBER; ++i) {
			update_icon_zone(i);

			if (on_cursor[i])
				position2[i] = std::lerp(position2[i], 0.03, fw.FT(35));
			else
				position2[i] = std::lerp(position2[i], 0.0, fw.FT(35));
		}
	}



	void Render() {
		for (int i = 0; i < ICON_NUMBER; ++i) {
			init_transform();

			alpha = transparent;
			set_object_static(dy(rt(-1.0) + 0.15 + 0.25 * i), dy(position + position2[i]));

			draw_image(tex[i]);
		}
	}


	IconHomeMenu() {
		for (int i = 0; i < ICON_NUMBER; ++i) {
			set_texture(tex[i], directory[i], 256, 256, 1);
		}
	}

	~IconHomeMenu() {
		for (int i = 0; i < ICON_NUMBER; ++i)
			glDeleteTextures(1, &tex[i]);
	}
};



#define BUTTON_NUMBER 2

class Button {
private:
	std::array<unsigned int, BUTTON_NUMBER> tex{};
	std::array<const char*, BUTTON_NUMBER> directory = {
		"res//ui//icon_off.png",
		"res//ui//icon_info.png",
	};


	std::array< std::array<GLfloat, 4>, BUTTON_NUMBER> button_zone{};


	std::array<bool, BUTTON_NUMBER> on_cursor{};
	std::array<bool, BUTTON_NUMBER> on_click{};


	// 아이콘 투명도
	GLfloat transparent{};

	// 아이콘 위치
	GLfloat position{};
	GLfloat position2[BUTTON_NUMBER]{};

	bool info_is_open{};

	Text* text = nullptr;

	int height = HEIGHT;

	GLfloat update_delay{};


public:
	// 메뉴 배경의 위치를 아이콘에게 주기
	void tell_position(GLfloat pos) { position = pos; }

	// 아이콘 개수 리턴
	size_t get_button_number() const { return BUTTON_NUMBER; }

	// 아이콘 위에 커서가 있음을 알림
	void tell_on_cursor(int idx) { on_cursor[idx] = true; }

	// 커서가 아이콘을 벗어났음을 알림
	void tell_not_on_cursor(int idx) { on_cursor[idx] = false; }

	void tell_info_is_open() { info_is_open = true; }
	void tell_info_is_close() { info_is_open = false; }

	// 아이콘 커서 인식 범위 리턴
	std::array<GLfloat, 4> get_button_zone(int i) const { return button_zone[i]; }

	// 마우스가 아이콘을 클릭했음을 알림
	void tell_button_click() {
		for (int i = 0; i < BUTTON_NUMBER; ++i) {
			if (on_cursor[i]) {
				on_click[i] = true;
				ssys_ui->playSound(menu_click, 0, false, &ch_ui);

				switch (i) {
				case 0:  // exit game
					glutDestroyWindow(1);
					break;
				
				case 1: // info
					if(!info_is_open)
						fw.AddSubObj(new Info(sub_layer1, "info"), sub_layer1);
					break;
				}

				break;
			}
		}
	}


	// 아이콘 커서 인식 범위 업데이트
	void update_button_zone(int i) {
		GLfloat x_min = rt(-1.0) + 0.1 - 0.08 + 0.01;
		GLfloat x_max = rt(-1.0) + 0.1 + 0.08 - 0.01;
		GLfloat y_min = position + 0.25 - 0.08 + 0.01 + 0.16 * i;
		GLfloat y_max = position + 0.25 + 0.08 - 0.01 + 0.16 * i;

		button_zone[i] = {dy(x_min), dy(x_max), dy(y_min), dy(y_max)};
	}


	void Update() {
		if (height != HEIGHT) {
			if(text != nullptr)
				delete text;
			text = nullptr;
			text = new Text("Maniac", 60, FW_DONTCARE);

			height = HEIGHT;
		}

		transparent = std::lerp(transparent, 1.0, fw.FT(7));

		// 커서를 마우스 위에 올리면 아이콘이 위로 올라오며 표시된다
		for (int i = 0; i < BUTTON_NUMBER; ++i) {
			update_button_zone(i);

			if (on_cursor[i])
				position2[i] = std::lerp(position2[i], 0.03, fw.FT(35));
			else
				position2[i] = std::lerp(position2[i], 0.0, fw.FT(35));
		}
	}



	void Render() {
		for (int i = 0; i < BUTTON_NUMBER; ++i) {
			init_transform();

			s_mat *= scale_image(0.8, 0.8);

			alpha = transparent;
			set_object_static(dy(rt(-1.0) + 0.1 + position2[i]), dy(position + 0.25 + 0.16 * i));

			draw_image(tex[i]);

			if (on_cursor[i]) {
				switch (i) {
				case 1:
					init_transform(); 
					text->out_static(rt(-1.0) + 0.4, position + 0.2, 1.0, 1.0, 1.0, "INFO");
					break;

				case 0:
					init_transform();
					text->out_static(rt(-1.0) + 0.4, position + 0.2, 1.0, 1.0, 1.0, "EXIT");
					break;
				}
			}
		}
	}


	Button() {
		for (int i = 0; i < BUTTON_NUMBER; ++i)
			set_texture(tex[i], directory[i], 512, 512, 1);
		text = new Text("Maniac", 60, FW_DONTCARE);
	}

	~Button() {
		delete text;
		text = nullptr;

		for(int i = 0; i < BUTTON_NUMBER; ++i)
			glDeleteTextures(1, &tex[i]);
	}
};