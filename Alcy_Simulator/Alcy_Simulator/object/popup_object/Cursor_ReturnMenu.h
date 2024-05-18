#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class CursorReturnMenu : public SUB_CLS {
private:
	unsigned int tex;

	int layer{};
	std::string tag;


	// 커서 위치
	GLfloat x{}, y{};


public:
	std::string GetTag() const { return tag; }

	void mouse_left_button_down(int button, int state) {
		auto ptr = fw.FindSubObj_Layer_Single(sub_layer1, "return_menu");
		if (ptr) ptr->tell_icon_click_return_menu();
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
		auto ptr = fw.FindSubObj_Layer_Single(sub_layer1, "return_menu");
		if (ptr) {
			if (check_dot_box_collision(x, y, ptr->get_icon_zone_return_menu()))
				ptr->tell_on_cursor_return_menu();

			else
				ptr->tell_not_on_cursor_return_menu();
		}
	}


	CursorReturnMenu(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//cursor.png", 200, 200, 1);
	}

	~CursorReturnMenu() {
		glDeleteTextures(1, &tex);
	}
};
