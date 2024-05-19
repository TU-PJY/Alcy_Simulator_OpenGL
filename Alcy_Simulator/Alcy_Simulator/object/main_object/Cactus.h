#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"



class Cactus : public MAIN_CLS {
private:
	unsigned int tex{};
	std::array<const char*, 8> directory = {
		"res//prop//object//game1_cactus_1.png",
		"res//prop//object//game1_cactus_2.png",
		"res//prop//object//game1_cactus_3.png",
		"res//prop//object//game1_cactus_4.png",
		"res//prop//object//game1_cactus_5.png",
		"res//prop//object//game1_cactus_6.png",
		"res//prop//object//game1_cactus_7.png",
		"res//prop//object//game1_cactus_8.png",
	};

	int layer{};
	std::string tag{};

	GLfloat x{};

	int number{};

	GLfloat move_speed{};

public:
	void Update() {
		auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "game1_controller");

		if (ptr && ptr->get_game1_play_state()) {
			auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_sprite");
			if (ptr) move_speed = ptr->get_alcy_sprite_move_speed();
			x -= fw.FT(move_speed);

			if (x <= -0.6)
				fw.DeleteMainObj(this, layer);
		}
	}

	void Render() {
		init_transform();

		t_mat *= move_image(x, 0.33);
		//s_mat *= scale_image(0.8, 0.8);
		

		draw_image(tex);
	}

	void CheckCollision() {

	}


	Cactus(int l, std::string str) {
		layer = l;
		tag = str;

		x = 0.6;

		std::random_device rd;  std::mt19937 gen(rd());
		std::uniform_int_distribution <int> dis(0, 7);

		number = dis(gen);

		set_texture(tex, directory[number], 150, 150, 1);
	}

	~Cactus() {
		glDeleteTextures(1, &tex);
	}
};
