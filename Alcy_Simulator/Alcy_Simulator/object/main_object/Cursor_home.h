#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../header/Camera.h"


class Cursor_home : public FUNCTION {
private:
	int layer{};
	std::string tag{};

	GLuint VAO{};
	std::array<unsigned int, 3> tex{};
	std::array<const char*, 3> directory = {
		"res//ui//cursor.png",
		"res//ui//cursor_hand.png",
		"res//ui//cursor_finger.png",
	};

	// Ŀ�� ��ġ
	GLfloat x{}, y{};

	// Ŀ�� �ؽ�ó ��ȣ
	int tex_number{};

	// ���ٵ�� ����
	bool touch_state{};

	// ���ٵ�� �� ����ϴ� Ŀ�� x ��ġ �� ���� ����
	GLfloat touch_x{};
	GLfloat touch_number{};

	// ���콺 Ŀ�� ���̴� ����
	bool visible = true;


public:
	GLfloat get_cursor_x() const { return x; }
	GLfloat get_cursor_y() const { return y; }
	bool get_touch_state() const { return touch_state; }
	GLfloat get_cursor_touch_x() const { return touch_x; }
	void set_cursor_invisible() { visible = false; }
	void set_cursor_visible() { visible = true; }


	// Ŀ�� Ÿ�� ������Ʈ
	void update_cursor_type() {
		// Ȩ ��� ����
		auto ptr = fw.get_ptr(alcy_layer, 0);

		if (touch_state) {
			tex_number = 1;
			return;
		}

		if (ptr != nullptr) {
			std::array<GLfloat, 4> touch_zone = ptr->get_touch_zone();
			std::array<GLfloat, 4> squeak_zone = ptr->get_squeak_zone();

			// Ư�� ������ Ŀ���� ��ġ�ϸ� Ŀ���� �ٲ��.
			// ��Ű ���ٵ�� ����
			if (ptr->get_interaction_available_state()) {
				if (touch_zone[0] < x && x < touch_zone[1] && touch_zone[2] < y && y < touch_zone[3])
					tex_number = 1;

				else if (squeak_zone[0] < x && x < squeak_zone[1] && squeak_zone[2] < y && y < squeak_zone[3])
					tex_number = 2;

				else
					tex_number = 0;
			}

			// Ŀ�� - ��Ű ��ȣ�ۿ� ���°� �ƴ϶�� �⺻ Ŀ���� �����Ѵ�
			else
				tex_number = 0;
		}
	}


	// ���콺 Ŭ�� �ʱ�ȭ
	void reset_mouse_state() {
		auto ptr = fw.get_ptr(alcy_layer, 0);

		if (ptr != nullptr && touch_state) {
			touch_state = false;
			ptr->tell_touch_state(touch_state);

			touch_x = 0;
			touch_number = 0;
		}
	}


	// ���콺 ���� ��ư Ŭ��
	void mouse_left_button_down(int button, int state) {
		auto ptr = fw.get_ptr(alcy_layer, 0);

		if (ptr != nullptr) {
			if(ptr->get_interaction_available_state()) {
				if (tex_number == 1) {
					touch_state = true;
					ptr->tell_touch_state(touch_state);
				}

				else if (tex_number == 2) {
					ptr->tell_squeak_state(true);
				}
			}
		}
	}


	// ���콺 ���� ��ư ������
	void mouse_left_button_up(int button, int state) {
		auto ptr = fw.get_ptr(alcy_layer, 0);

		if (ptr != nullptr) {
			if (touch_state) {
				touch_state = false;
				ptr->tell_touch_state(touch_state);

				touch_x = 0;
				touch_number = 0;
			}
		}
	}


	// ��Ű ���ٵ�� Ŀ�� ������ ������Ʈ
	void move_cursor_touch_state() {
		if (touch_state) {
			touch_number += fw.calc_ft(2.5);
			touch_x = sin(touch_number) / 4;
		}
	}


	void update() {
		update_cursor_type();
		move_cursor_touch_state();

		if (touch_state) {
			x = touch_x;
			y = 0.4;
		}

		else {
			x = mx * ratio / cam.zoom;
			y = my / cam.zoom;
		}
	}


	void check_collision() {
		update_cursor_type();
	}


	void render() {
		init_transform();
		set_object_static(x, y);

		if(visible)
			draw_image(tex[tex_number], VAO);
	}


	void check_delete() {

	}


	Cursor_home(int l, std::string str) {
		layer = 1;
		tag = str;

		set_canvas(VAO);
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 200, 200, 1);
	}
};