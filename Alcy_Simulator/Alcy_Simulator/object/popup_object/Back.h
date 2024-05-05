#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Back : public POPUP_FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex{};

	int layer{};
	std::string tag{};

	GLfloat transparent{};


public:
	void update() {
		transparent = std::lerp(transparent, 0.7, fw.calc_ft(10));
	}

	void render() {
		init_transform();
		s_mat *= scale_image(10.0 * ratio, 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex, VAO);
	}

	void check_collision() {}

	void check_delete() {}

	Back(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//ui//black.png", 100, 100, 1);
	}
};