#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// 메뉴 종료 후 추가하는 배경
class Front : public MAIN_CLS {
private:
	unsigned int tex{};

	int layer{};
	std::string tag{};


	bool delete_flag{};


	GLfloat transparent = 1.5;

public:
	void Update() {
		// 완전히 투명해지면 스스로 삭제
		transparent = std::lerp(transparent, 0.0, fw.FT(3));

		if (transparent <= 0.0001)
			fw.DeleteMainObj(this, layer);
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(rt(10.0), 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex);
	}

	void CheckCollision() {}


	Front (int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//black.png", 100, 100, 1);
	}

	~Front() {
		glDeleteTextures(1, &tex);
	}
};