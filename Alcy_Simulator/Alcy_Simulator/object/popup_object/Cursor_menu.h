#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Cursor_menu : public SUB_CLS {
private:
	unsigned int tex;

	int layer{};
	std::string tag;


	// 커서 위치
	GLfloat x{}, y{};


public:
	std::string GetTag() const { return tag; }

	void mouse_left_button_down(int button, int state) {
		// info 닫기
		auto ptr2 = fw.FindSubObj(sub_layer1, "info");
		if (ptr2 != nullptr && ptr2->get_info_visible_state())
			ptr2->close_info();

		// button, icon 클릭
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
		// 커서를 아이콘 위에 올리면 표시된다.
		// 커서가 아이콘을 벗어나면 아이콘 위치가 다시 복구된다.
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

		// 메뉴를 연 동안에는 메인 모드에 존재하는 커서를 보이지 않게한다.
		// 메인 모드의 커서와 다른 객체임
		auto ptr = fw.FindMainObj(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->set_cursor_invisible();
	}

	~Cursor_menu() {
		// 메뉴를 종료하면 다시 메인 모드의 커서를 보이게 한다.
		auto ptr = fw.FindMainObj(main_layer7, "cursor_home");
		if (ptr != nullptr)
			ptr->set_cursor_visible();
	}
};