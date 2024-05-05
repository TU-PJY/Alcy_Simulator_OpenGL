#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// 환영 메시지
class Welcome_messege : public FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex{};
	
	int layer{};
	std::string tag{};

	GLfloat size{};
	GLfloat num{};
	GLfloat num2 = 1.5;
	GLfloat num3{};

	GLfloat transparent = 1.0;

	GLfloat timer{};

	bool delete_flag{};

public:
	void update() {
		if (num2 != 0) {
			num3 += fw.calc_ft(20);

			if (num3 > 2.5) {
				num3 = 2.5;

				num += fw.calc_ft(20);
				num2 -= fw.calc_ft(2);

				if (num2 < 0)  num2 = 0;
			}
		}

		if (num2 == 0) {
			timer += fw.calc_ft(1);
			if (timer > 1.0) {
				num3 = std::lerp(num3, 0.0, fw.calc_ft(10));
				transparent = std::lerp(transparent, 0.0, fw.calc_ft(10));

				if (num3 <= 0.001)  delete_flag = true;
			}
		}

		size = num3 + sin(num) * num2;
	}

	void render() {
		init_transform();
		s_mat *= scale_image(size, size);
		t_mat *= move_image(0.65, 0.5);
		alpha = transparent;

		draw_image(tex, VAO);
	}

	void check_collision(){}

	void check_delete() {
		if (delete_flag)
			fw.delete_object(this, layer);
	}

	Welcome_messege(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//prop//welcome_messege.png", 256, 256, 1);
	}
};