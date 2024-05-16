#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../header/Camera.h"


class Cursor_home : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

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

	GLfloat transparent = 1.0;


public:
	GLfloat get_cursor_x() const { return x; }
	GLfloat get_cursor_y() const { return y; }
	bool get_touch_state() const { return touch_state; }
	GLfloat get_cursor_touch_x() const { return touch_x; }
	void set_cursor_invisible() { visible = false; }
	void set_cursor_visible() { visible = true; }

	std::string GetTag() const { return tag; }


	// Ŀ�� Ÿ�� ������Ʈ
	void update_cursor_type() {
		// Ȩ ��� ����
		auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_home");

		if (!touch_state) {
			if (cam.key_state_left || cam.key_state_right) {
				tex_number = 0;
				return;
			}
		}

		if (touch_state) {
			tex_number = 1;
			return;
		}

		if (ptr != nullptr) {
			// Ư�� ������ Ŀ���� ��ġ�ϸ� Ŀ���� �ٲ��.
			// ��Ű ���ٵ�� ����
			if (ptr->get_interaction_available_state()) {
				if(check_dot_box_collision(x, y, ptr->get_touch_zone()))
					tex_number = 1;

				else if (check_dot_box_collision(x, y, ptr->get_squeak_zone()))
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
		auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_home");

		if (ptr != nullptr && touch_state) {
			touch_state = false;
			ptr->tell_touch_state(touch_state);

			touch_x = 0;
			touch_number = 0;
		}
	}


	// ���콺 ���� ��ư Ŭ��
	void mouse_left_button_down(int button, int state) {
		auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_home");

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
		auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_home");

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
			touch_number += fw.FT(2.5);
			touch_x = sin(touch_number) / 4;
		}
	}


	void Update() {
		update_cursor_type();
		move_cursor_touch_state();

		if (touch_state) {
			x = touch_x;
			y = 0.4;
		}

		else {
			x = dy(rt(mx));
			y = dy(my);
		}

		// ī�޶� ����̴� ���ȿ��� Ŀ���� ǥ������ �ʴ´�.
		auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_home");
		if (cam.key_state_left || cam.key_state_right || (ptr && ptr->get_head_rotate_state() != 2)) {
			transparent = std::lerp(transparent, 0.0, fw.FT(15));
		}

		else 
			transparent = std::lerp(transparent, 1.0, fw.FT(15));
	}


	void CheckCollision() {
		update_cursor_type();
	}


	void Render() {
		init_transform();
		set_object_static(x, y);

		alpha = transparent;

		if(visible)
			draw_image(tex[tex_number]);
	}


	void CheckDelete() {

	}


	Cursor_home(int l, std::string str) {
		layer = 1;
		tag = str;

		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 200, 200, 1);
	}
};