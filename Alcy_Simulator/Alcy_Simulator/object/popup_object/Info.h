#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Info : public SUB_CLS {
private:
	unsigned int tex{};

	int layer{};
	std::string tag{};

	GLfloat transparent = 0.0;

	GLfloat size = 10.0;

	bool visible = true;

public:
	bool get_info_visible_state() { return visible; }

	void close_info() {visible = false;}

	std::string GetTag() const { return tag; }

	void Update() {
		if (visible) {
			size = std::lerp(size, 7.0, fw.FT(20));
			transparent = std::lerp(transparent, 1.0, fw.FT(20));
		}

		else {
			size = std::lerp(size, 10.0, fw.FT(20));
			transparent = std::lerp(transparent, 0.0, fw.FT(20));

			if (transparent <= 0.001)
				fw.DeleteSubObj(this, layer);
		}
	}

	void Render() {
		init_transform();

		s_mat *= scale_image(size, size);
		alpha = transparent;
		set_object_static(0.0, 0.0);

		draw_image(tex);
	}

	void CheckCollision() {

	}

	void CheckDelete() {

	}

	Info(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//info.png", 1024, 1024, 1);

		auto ptr = fw.FindSubObj_Layer_Single(sub_layer1, "menu");
		if (ptr != nullptr)
			ptr->tell_info_is_open();
	}


	~Info() {
		auto ptr = fw.FindSubObj_Layer_Single(sub_layer1, "menu");
		if (ptr != nullptr)
			ptr->tell_info_is_close();
	}
};