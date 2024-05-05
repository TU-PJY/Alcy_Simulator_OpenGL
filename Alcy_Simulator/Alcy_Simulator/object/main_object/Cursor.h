#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../header/Camera.h"


class Cursor : public FUNCTION {
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

	GLfloat x{}, y{};

public:
	GLfloat get_cursor_x() { return x; }
	GLfloat get_cursor_y() { return y; }

	void update() {
		x = mx * ratio;
		y = my;
	}

	void check_collision() {

	}

	void render() {
		init_transform();
		set_object_static(x, y);
		draw_image(tex[0], VAO);
	}

	void check_delete() {

	}

	Cursor(int l, std::string str) {
		layer = 1;
		tag = str;

		set_canvas(VAO);
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 200, 200, 1);
	}
};