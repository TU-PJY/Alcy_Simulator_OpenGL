#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Cursor_menu : public POPUP_FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex;

	int layer{};
	std::string tag;


	// Ŀ�� ��ġ
	GLfloat x{}, y{};


public:
	void update() {
		x = mx * ratio / cam.zoom;
		y = my / cam.zoom;
	}

	void render() {
		init_transform();
		set_object_static(x, y);
		draw_image(tex, VAO);
	}

	void check_collision() {
		// Ŀ���� ������ ���� �ø��� ǥ�õȴ�.
		// Ŀ���� �������� ����� ������ ��ġ�� �ٽ� �����ȴ�.
		auto ptr = fw.get_popup_ptr(layer1, 1);
		if (ptr != nullptr) {
			for (int i = 0; i < ptr->get_icon_number(); ++i) {
				if (ptr->get_icon_zone()[0] + 0.25 * i < x && x < ptr->get_icon_zone()[1] + 0.25 * i &&
					ptr->get_icon_zone()[2] < y && y < ptr->get_icon_zone()[3])

					ptr->tell_on_cursor(i);

				else
					ptr->tell_not_on_cursor(i);
			}
		}
	}

	void check_delete() {};

	Cursor_menu(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//ui//cursor.png", 200, 200, 1);

		// �޴��� �� ���ȿ��� ���� ��忡 �����ϴ� Ŀ���� ������ �ʰ��Ѵ�.
		// ���� ����� Ŀ���� �ٸ� ��ü��
		auto ptr = fw.get_ptr(cursor_layer, 0);
		if (ptr != nullptr)
			ptr->set_cursor_invisible();
	}

	~Cursor_menu() {
		// �޴��� �����ϸ� �ٽ� ���� ����� Ŀ���� ���̰� �Ѵ�.
		auto ptr = fw.get_ptr(cursor_layer, 0);
		if (ptr != nullptr)
			ptr->set_cursor_visible();
	}
};