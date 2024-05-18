#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"


class Shadow : public MAIN_CLS {
private:
	int layer{};
	std::string tag;

	unsigned int tex;

public:
	void Update(){}

	void Render() {
		init_transform();

		s_mat *= scale_image(5.0, 4.0);
		t_mat *= move_image(0.0, -0.97);
		draw_image(tex);
	}

	void CheckCollision(){}

	Shadow(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//prop//object//shadow.png", 256, 256, 1);
	}

	~Shadow() {
		glDeleteTextures(1, &tex);
	}
};