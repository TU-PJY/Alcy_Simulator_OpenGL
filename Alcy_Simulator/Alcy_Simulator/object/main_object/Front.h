#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// �޴� ���� �� �߰��ϴ� ���
class Front : public MAIN_CLS {
private:
	unsigned int tex{};

	int layer{};
	std::string tag{};


	bool delete_flag{};


	GLfloat transparent = 1.0;

public:
	void Update() {
		// ������ ���������� ������ ����
		transparent = std::lerp(transparent, 0.0, fw.FT(3));

		if (transparent <= 0.0001)
			delete_flag = true;
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(rt(10.0), 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex);
	}

	void CheckCollision() {}


	void CheckDelete() {
		if (delete_flag)
			fw.DeleteMainObj(this, layer);
	}


	Front (int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//black.png", 100, 100, 1);
	}
};