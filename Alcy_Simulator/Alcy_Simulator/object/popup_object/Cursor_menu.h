#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Cursor_menu : public POPUP_FUNCTION {
private:
	unsigned int tex;

	int layer{};
	std::string tag;


	// Ŀ�� ��ġ
	GLfloat x{}, y{};


public:
	std::string get_tag() const { return tag; }

	void mouse_left_button_down(int button, int state) {
		// info �ݱ�
		auto ptr2 = fw.find_popup_object(popup_layer1, "info");
		if (ptr2 != nullptr && ptr2->get_info_visible_state())
			ptr2->close_info();

		// button, icon Ŭ��
		auto ptr = fw.find_popup_object(popup_layer1, "menu");
		if (ptr != nullptr) {
			ptr->tell_icon_click();
			ptr->tell_button_click();
		}
	}


	void update() {
		x = mx * ratio / cam.zoom;
		y = my / cam.zoom;
	}


	void render() {
		init_transform();
		set_object_static(x, y);
		draw_image(tex);
	}


	void check_collision() {
		// Ŀ���� ������ ���� �ø��� ǥ�õȴ�.
		// Ŀ���� �������� ����� ������ ��ġ�� �ٽ� �����ȴ�.
		auto ptr = fw.find_popup_object(popup_layer1, "menu");

		if (ptr != nullptr) {
			// icon
			for (int i = 0; i < ptr->get_icon_number(); ++i) {
				GLfloat cx = 0.25 * i;
				if (set_dy(ptr->get_icon_zone()[0]+cx) < x && 
					set_dy(ptr->get_icon_zone()[1]+cx) > x &&
					set_dy(ptr->get_icon_zone()[2]) < y && 
					set_dy(ptr->get_icon_zone()[3]) > y)

					ptr->tell_on_cursor(i);

				else
					ptr->tell_not_on_cursor(i);

			}

			// button
			for (int i = 0; i < ptr->get_button_number(); ++i) {
				GLfloat cy = 0.16 * i;
				if (set_dy(ptr->get_button_zone()[0]) < x && 
					set_dy(ptr->get_button_zone()[1]) > x &&
					set_dy(ptr->get_button_zone()[2]+cy) < y && 
					set_dy(ptr->get_button_zone()[3]+cy) > y)

					ptr->tell_on_cursor_button(i);

				else
					ptr->tell_not_on_cursor_button(i);
			}
			
		}
	}


	void check_delete() {};



	Cursor_menu(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//cursor.png", 200, 200, 1);

		// �޴��� �� ���ȿ��� ���� ��忡 �����ϴ� Ŀ���� ������ �ʰ��Ѵ�.
		// ���� ����� Ŀ���� �ٸ� ��ü��
		auto ptr = fw.find_object(layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->set_cursor_invisible();
	}

	~Cursor_menu() {
		// �޴��� �����ϸ� �ٽ� ���� ����� Ŀ���� ���̰� �Ѵ�.
		auto ptr = fw.find_object(layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->set_cursor_visible();
	}
};