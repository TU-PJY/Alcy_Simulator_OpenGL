#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"
#include "Info.h"

// ������ ����
#define ICON_NUMBER 4

class Icon {
private:
	std::array<unsigned int, ICON_NUMBER> tex{};
	std::array<const char*, ICON_NUMBER> directory = {
		"res//ui//menu//icon1.png",
		"res//ui//menu//icon2.png",
		"res//ui//menu//icon3.png",
		"res//ui//menu//icon4.png",
	};

	// Ŀ���� ������ ���� �ִ��� ����
	std::array<bool, ICON_NUMBER> on_cursor{};
	std::array<bool, ICON_NUMBER> on_click{};

	// ������ Ŀ�� �ν� ����
	std::array< std::array<GLfloat, 4>, ICON_NUMBER> icon_zone{};

	// ������ ����
	GLfloat transparent{};

	// ������ ��ġ
	GLfloat position{};
	GLfloat position2[ICON_NUMBER]{};


public:
	// �޴� ����� ��ġ�� �����ܿ��� �ֱ�
	void tell_position(GLfloat pos) { position = pos; }

	// ������ ���� ����
	size_t get_icon_number() const { return ICON_NUMBER; }

	// ������ ���� Ŀ���� ������ �˸�
	void tell_on_cursor(int idx) { on_cursor[idx] = true; }

	// Ŀ���� �������� ������� �˸�
	void tell_not_on_cursor(int idx) { on_cursor[idx] = false; }

	// ���콺�� �������� Ŭ�������� �˸�
	void tell_icon_click() { 
		for (int i = 0; i < ICON_NUMBER; ++i) {
			if (on_cursor[i]) {
				on_click[i] = true;
				ssys_ui->playSound(menu_click, 0, false, &ch_ui);

				
				on_click[i] = false;
			}
		}
	}

	// ������ Ŀ�� �ν� ���� ����
	std::array<GLfloat, 4> get_icon_zone(int i) const { return icon_zone[i]; }
	

	// ������ Ŀ�� �ν� ���� ������Ʈ
	void update_icon_zone(int i) {
		GLfloat x_min = rt(-1.0) + 0.05 + 0.01 + 0.25 * i;
		GLfloat x_max = rt(-1.0) + 0.25 - 0.01 + 0.25 * i;
		GLfloat y_min = position - 0.1 + 0.01;
		GLfloat y_max = position + 0.1 - 0.01;

		icon_zone[i] = {dy(x_min), dy(x_max), dy(y_min), dy(y_max)};
	}


	void Update() {

		transparent = std::lerp(transparent, 1.0, fw.FT(7));

		// Ŀ���� ���콺 ���� �ø��� �������� ���� �ö���� ǥ�õȴ�
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



	Icon() {
		for (int i = 0; i < ICON_NUMBER; ++i)
			set_texture(tex[i], directory[i], 256, 256, 1);
	}
};

#define BUTTON_NUMBER 2

class Button {
private:
	std::array<unsigned int, BUTTON_NUMBER> text_tex{};
	std::array<const char*, BUTTON_NUMBER> text_directory = {
		"res//ui//text//text_quit.png",
		"res//ui//text//text_info.png",
	};

	std::array<unsigned int, BUTTON_NUMBER> tex{};
	std::array<const char*, BUTTON_NUMBER> directory = {
		"res//ui//icon_off.png",
		"res//ui//icon_info.png",
	};


	std::array< std::array<GLfloat, 4>, BUTTON_NUMBER> button_zone{};


	std::array<bool, BUTTON_NUMBER> on_cursor{};
	std::array<bool, BUTTON_NUMBER> on_click{};


	// ������ ����
	GLfloat transparent{};

	// ������ ��ġ
	GLfloat position{};
	GLfloat position2[BUTTON_NUMBER]{};

	bool info_is_open{};


public:
	// �޴� ����� ��ġ�� �����ܿ��� �ֱ�
	void tell_position(GLfloat pos) { position = pos; }

	// ������ ���� ����
	size_t get_button_number() const { return tex.size(); }

	// ������ ���� Ŀ���� ������ �˸�
	void tell_on_cursor(int idx) { on_cursor[idx] = true; }

	// Ŀ���� �������� ������� �˸�
	void tell_not_on_cursor(int idx) { on_cursor[idx] = false; }

	void tell_info_is_open() { info_is_open = true; }
	void tell_info_is_close() { info_is_open = false; }

	// ������ Ŀ�� �ν� ���� ����
	std::array<GLfloat, 4> get_button_zone(int i) const { return button_zone[i]; }

	// ���콺�� �������� Ŭ�������� �˸�
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

				on_click[i] = false;
			}
		}
	}


	// ������ Ŀ�� �ν� ���� ������Ʈ
	void update_button_zone(int i) {
		GLfloat x_min = rt(-1.0) + 0.1 - 0.08 + 0.01;
		GLfloat x_max = rt(-1.0) + 0.1 + 0.08 - 0.01;
		GLfloat y_min = position + 0.25 - 0.08 + 0.01 + 0.16 * i;
		GLfloat y_max = position + 0.25 + 0.08 - 0.01 + 0.16 * i;

		button_zone[i] = {dy(x_min), dy(x_max), dy(y_min), dy(y_max)};
	}


	void Update() {

		transparent = std::lerp(transparent, 1.0, fw.FT(7));

		// Ŀ���� ���콺 ���� �ø��� �������� ���� �ö���� ǥ�õȴ�
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

			init_transform();
			s_mat *= scale_image(1.5, 1.5);
			set_object_static(dy(rt(-1.0) + 0.5), dy(position + 0.235));

			if (on_cursor[i])
				draw_image(text_tex[i]);
		}
	}


	Button() {
		for (int i = 0; i < BUTTON_NUMBER; ++i)
			set_texture(tex[i], directory[i], 512, 512, 1);

		for (int i = 0; i < BUTTON_NUMBER; ++i)
			set_texture(text_tex[i], text_directory[i], 512, 512, 1);
	}

};