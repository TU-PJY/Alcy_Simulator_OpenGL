#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"


class BackGround_home : public FUNCTION{
private:
	GLuint VAO;
	unsigned int tex;

	int layer{};
	std::string tag;

public:
	void update() {}

	void render() {
		init_transform();
		
		s_mat *= scale_image(25.0 * ratio, 25.0);
		t_mat *= move_image(-cam.x / 2, -cam.y / 2);
		draw_image(tex, VAO);
	}

	void check_collision() {}

	void check_delete() {}


	BackGround_home(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//background//default.png", 1440, 1440, 1);
	}
};