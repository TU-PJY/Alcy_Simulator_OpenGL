#pragma once
#include "../../header/image_text_util.h"
#include "../../header/Camera.h"
#include "../../header/view.h"


class Tip_home : public FUNCTION {
private:
	unsigned int tex{};

	int layer{};
	std::string tag{};

	GLfloat size = 3.0;
	GLfloat transparent = 1.0;

	bool visible = true;

public:
	std::string get_tag() const { return tag; }

	void update_tip_visible (unsigned char KEY) {
		if (KEY == 9) {
			if (visible)
				visible = false;
			else
				visible = true;

			ssys_ui->playSound(tip_click, 0, false, &ch_ui);
		}
	}


	void update() {
		if (visible) {
			transparent = std::lerp(transparent, 1.0, fw.calc_ft(15));
			size = std::lerp(size, 3.0, fw.calc_ft(15));
		}
		else {
			transparent = std::lerp(transparent, 0.0, fw.calc_ft(15));
			size = std::lerp(size, 4.0, fw.calc_ft(15));
		}
	}


	void render() {
		init_transform();
		alpha = transparent;

		s_mat *= scale_image(size, size);
		set_object_static(dy(rt(1.0) - 0.3), dy(-0.7));

		if(fw.get_current_mode() == "home_mode")
			draw_image(tex);
	}

	void check_collision() {}

	void check_delete() {}

	Tip_home(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//tip.png", 500, 500, 1);
	}
};