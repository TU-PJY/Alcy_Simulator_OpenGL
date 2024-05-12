#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// 메뉴 종료 후 추가하는 배경
class Front : public FUNCTION {
private:
	unsigned int tex{};

	int layer{};
	std::string tag{};


	bool delete_flag{};


	GLfloat transparent = 1.0;

public:
	void update() {
		// 완전히 투명해지면 스스로 삭제
		transparent = std::lerp(transparent, 0.0, fw.calc_ft(3));

		if (transparent <= 0.0001)
			delete_flag = true;
	}

	void render() {
		init_transform();
		s_mat *= scale_image(10.0 * ratio, 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex);
	}

	void check_collision() {}


	void check_delete() {
		if (delete_flag)
			fw.delete_object(this, layer);
	}


	Front (int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//black.png", 100, 100, 1);
	}
};