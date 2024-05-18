#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"



class Cactus : public MAIN_CLS {
private:
	unsigned int tex{};
	std::array<const char*, 3> directory = {
		"res//prop//object//game1_cactus_1.png",
		"res//prop//object//game1_cactus_2.png",
		"res//prop//object//game1_cactus_3.png",
	};

	std::array<int, 3> size = { 75, 128, 128 };

	int layer{};
	std::string tag{};

	GLfloat x{};

	int number{};

public:
	void Update() {
		x -= fw.FT(0.3);

		if (x <= -0.6)
			fw.DeleteMainObj(this, layer);
	}

	void Render() {
		init_transform();

		if (number == 0) {
			t_mat *= move_image(x, 0.47);
			s_mat *= scale_image(0.5, 0.5);
		}

		else {
			t_mat *= move_image(x, 0.5);
			s_mat *= scale_image(0.8, 0.8);
		}

		draw_image(tex);
	}

	void CheckCollision() {

	}


	Cactus(int l, std::string str) {
		layer = l;
		tag = str;

		x = 0.6;

		std::random_device rd;  std::mt19937 gen(rd());
		std::uniform_int_distribution <int> dis(0, 2);

		number = dis(gen);

		set_texture(tex, directory[number], size[number], size[number], 1);
	}

	~Cactus() {
		glDeleteTextures(1, &tex);
	}
};
