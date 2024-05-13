#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Cursor_menu : public SUB_CLS {
private:
	unsigned int tex;

	int layer{};
	std::string tag;


	// Ŀ�� ��ġ
	GLfloat x{}, y{};


public:
	std::string GetTag() const { return tag; }

	void mouse_left_button_down(int button, int state) {
		// info �ݱ�
		auto ptr2 = fw.FindSubObj(sub_layer1, "info");
		if (ptr2 != nullptr && ptr2->get_info_visible_state())
			ptr2->close_info();

		// button, icon Ŭ��
		auto ptr = fw.FindSubObj(sub_layer1, "menu");
		if (ptr != nullptr) {
			ptr->tell_icon_click();
			ptr->tell_button_click();
		}
	}


	void Update() {
		x = dy(rt(mx));
		y = dy(my);
	}


	void Render() {
		init_transform();
		set_object_static(x, y);
		draw_image(tex);
	}


	void CheckCollision() {
		// Ŀ���� ������ ���� �ø��� ǥ�õȴ�.
		// Ŀ���� �������� ����� ������ ��ġ�� �ٽ� �����ȴ�.
		auto ptr = fw.FindSubObj(sub_layer1, "menu");

		if (ptr != nullptr) {
			// icon
			for (int i = 0; i < ptr->get_icon_number(); ++i) {
				if(check_dot_box_collision(x, y, ptr->get_icon_zone(i)))

					ptr->tell_on_cursor(i);

				else
					ptr->tell_not_on_cursor(i);

			}

			// button
			for (int i = 0; i < ptr->get_button_number(); ++i) {
				if (check_dot_box_collision(x, y, ptr->get_button_zone(i)))

					ptr->tell_on_cursor_button(i);

				else
					ptr->tell_not_on_cursor_button(i);
			}
			
		}
	}


	void CheckDelete() {};



	Cursor_menu(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//cursor.png", 200, 200, 1);

		// �޴��� �� ���ȿ��� ���� ��忡 �����ϴ� Ŀ���� ������ �ʰ��Ѵ�.
		// ���� ����� Ŀ���� �ٸ� ��ü��
		auto ptr = fw.FindMainObj(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->set_cursor_invisible();
	}

	~Cursor_menu() {
		// �޴��� �����ϸ� �ٽ� ���� ����� Ŀ���� ���̰� �Ѵ�.
		auto ptr = fw.FindMainObj(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->set_cursor_visible();
	}
};