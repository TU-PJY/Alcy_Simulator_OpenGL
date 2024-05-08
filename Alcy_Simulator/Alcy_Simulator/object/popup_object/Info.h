#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Info : public POPUP_FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex{};

	int layer{};
	std::string tag{};

	GLfloat transparent = 0.0;

	GLfloat size = 10.0;

	bool visible = true;

public:
	bool get_info_visible_state() { return visible; }

	void close_info() {visible = false;}

	std::string get_tag() const { return tag; }

	void update() {
		if (visible) {
			size = std::lerp(size, 7.0, fw.calc_ft(20));
			transparent = std::lerp(transparent, 1.0, fw.calc_ft(20));
		}

		else {
			size = std::lerp(size, 10.0, fw.calc_ft(20));
			transparent = std::lerp(transparent, 0.0, fw.calc_ft(20));

			if (transparent <= 0.001)
				fw.delete_popup_object(this, layer);
		}
	}

	void render() {
		init_transform();

		s_mat *= scale_image(size, size);
		alpha = transparent;
		set_object_static(0.0, 0.0);

		draw_image(tex, VAO);
	}

	void check_collision() {

	}

	void check_delete() {

	}

	Info(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//ui//info.png", 1024, 1024, 1);

		auto ptr = fw.find_popup_object(popup_layer1, "menu");
		if (ptr != nullptr)
			ptr->tell_info_is_open();
	}


	~Info() {
		auto ptr = fw.find_popup_object(popup_layer1, "menu");
		if (ptr != nullptr)
			ptr->tell_info_is_close();
	}
};