#pragma once
#include "../../header/Camera.h"
#include "../../header/image_text_util.h"
#include "../../header/sound.h"
#include "../../header/view.h"
#include "../../header/mode_header.h"

class ReturnIcon {
private:
	unsigned int tex{};

	// Ŀ���� ������ ���� �ִ��� ����
	bool on_cursor{};
	bool on_click{};

	// ������ Ŀ�� �ν� ����
	std::array<GLfloat, 4> icon_zone{};

	// ������ ����
	GLfloat transparent{};

	// ������ ��ġ
	GLfloat position{};
	GLfloat position2{};


public:
	// �޴� ����� ��ġ�� �����ܿ��� �ֱ�
	void tell_position (GLfloat pos) { position = pos; }

	// ������ ���� Ŀ���� ������ �˸�
	void tell_on_cursor() { on_cursor = true; }

	// Ŀ���� �������� ������� �˸�
	void tell_not_on_cursor() { on_cursor = false; }

	// ������ Ŀ�� �ν� ���� ����
	std::array<GLfloat, 4> get_icon_zone() const { return icon_zone; }


	// ���콺�� �������� Ŭ�������� �˸�
	void tell_icon_click() {
		if (on_cursor) {
			on_click = true;

			if (on_click) {
				// ���� ��尡 �����̳Ŀ� ���� ���ư��� ��尡 �޶�����
				if(fw.CurrentMainMode() == "game_mode")
					fw.SwitchMainMode(home_mode, "home_mode");
			}
		}
	}


	// ������ Ŀ�� �ν� ���� ������Ʈ
	void update_icon_zone() {
		GLfloat x_min = rt(-1.0) + 0.05 + 0.01;
		GLfloat x_max = rt(-1.0) + 0.25 - 0.01;
		GLfloat y_min = position - 0.1 + 0.01;
		GLfloat y_max = position + 0.1 - 0.01;

		icon_zone = { dy(x_min), dy(x_max), dy(y_min), dy(y_max) };
	}


	void Update() {
		transparent = std::lerp(transparent, 1.0, fw.FT(7));

		// Ŀ���� ���콺 ���� �ø��� �������� ���� �ö���� ǥ�õȴ�
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

