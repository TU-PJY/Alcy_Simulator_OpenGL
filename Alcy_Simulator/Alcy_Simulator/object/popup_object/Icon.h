#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"

// ������ ����
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

	// Ŀ���� ������ ���� �ִ��� ����
	std::array<bool, ICON_NUMBER> on_cursor{};
	std::array<bool, ICON_NUMBER> on_click{};

	// ������ Ŀ�� �ν� ����
	std::array<GLfloat, 4> icon_zone{};

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

				// �׽�Ʈ��
				fw.end_popup();
			}
		}
	}

	// ������ Ŀ�� �ν� ���� ����
	std::array<GLfloat, 4> get_icon_zone() const { return icon_zone; }
	

	// ������ Ŀ�� �ν� ���� ������Ʈ
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

		// Ŀ���� ���콺 ���� �ø��� �������� ���� �ö���� ǥ�õȴ�
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