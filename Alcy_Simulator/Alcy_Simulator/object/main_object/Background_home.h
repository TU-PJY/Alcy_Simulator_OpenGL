#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"


class BackGround_home : public FUNCTION{
private:
	unsigned int tex;

	int layer{};
	std::string tag;

public:
	void update() {}

	void render() {
		init_transform();
		
		s_mat *= scale_image(rt(25.0), 25.0);
		t_mat *= move_image(-cam.x / 2, -cam.y / 2);
		draw_image(tex);
	}

	void check_collision() {}

	void check_delete() {}


	BackGround_home(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//background//default.png", 1440, 1440, 1);
	}
};