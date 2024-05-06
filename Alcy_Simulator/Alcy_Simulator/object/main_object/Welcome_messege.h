#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"
#include "../../header/data_util.h"

// 환영 메시지
class Welcome_messege : public FUNCTION {
private:
	GLuint VAO{};
	std::array<unsigned int, 6> tex{};
	std::array<const char*, 6> directory = {
		"res//prop//welcome_messege_first.png",
		"res//prop//welcome_messege_again_1.png",
		"res//prop//welcome_messege_again_2.png",
		"res//prop//welcome_messege_again_3.png",
		"res//prop//welcome_messege_again_4.png",
		"res//prop//welcome_messege_again_5.png",
	};

	int number{};
	

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
			if (timer > 2.0) {
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
		t_mat *= move_image(0.75, 0.5);
		alpha = transparent;

		draw_image(tex[number], VAO);
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

		// 처음 게임을 실행할 때와 나중에 실행 할 때의 말풍선이 다르다
		if (load_data("play count") == 0) {
			set_texture(tex[0], directory[0], 256, 256, 1);
			update_data("play count", 1);
		}

		// 최초 실행 이후에는 5가지 말풍선 중 하나를 랜덤으로 출력한다
		else {
			std::random_device rd;  std::mt19937 gen(rd());
			std::uniform_int_distribution <int> dis(1, 5);

			number = dis(gen);
			set_texture(tex[number], directory[number], 256, 256, 1);
		}

		ssys->playSound(welcome, 0, false, &ch_alcy);
	}
};