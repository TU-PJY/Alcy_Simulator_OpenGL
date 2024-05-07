#pragma once
#include "../../header/image_text_util.h"
#include "../../header/Camera.h"
#include "../../header/view.h"


class Tip : public FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex{};

	int layer{};
	std::string tag{};

	GLfloat transparent = 1.0;

	bool visible = true;

public:
	void update() {
		if (visible)
			transparent = std::lerp(transparent, 1.0, fw.calc_ft(15));
		else
			transparent = std::lerp(transparent, 0.0, fw.calc_ft(15));
	}


	void render() {
		init_transform();

		s_mat *= scale_image(3.0, 3.0);
		set_object_static((-1.0 * ratio + 0.3) / cam.zoom, 0.7 / cam.zoom);
		draw_image(tex, VAO);
	}

	void check_collision() {}

	void check_delete() {}

	Tip(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//ui//tip.png", 500, 500, 1);
	}
};