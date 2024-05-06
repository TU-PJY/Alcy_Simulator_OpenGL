#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"


class Shadow : public FUNCTION {
private:
	int layer{};
	std::string tag;

	GLuint VAO;
	unsigned int tex;

public:
	void update(){}

	void render() {
		init_transform();

		s_mat *= scale_image(5.0, 4.0);
		t_mat *= move_image(0.0, -1.17);
		draw_image(tex, VAO);
	}

	void check_collision(){}

	void check_delete(){}


	Shadow(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//prop//shadow.png", 256, 256, 1);
	}
};