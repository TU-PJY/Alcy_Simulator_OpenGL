#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"

class Gameboy : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	unsigned int tex{};
	std::array<unsigned int, 2> tex_back{};
	std::array<unsigned int, 2> tex_hand{};

	bool power_on{};

public:
	std::string GetTag() const { return tag; }

	void Update() {}

	void Render() {
		init_transform();
		s_mat *= scale_image(10.0, 10.0);
		if (power_on)
			draw_image(tex_back[1]);
		else
			draw_image(tex_back[0]);


		init_transform();
		s_mat *= scale_image(15.0, 15.0);
		t_mat *= move_image(-1.3, -1.2);
		draw_image(tex_hand[0]);


		init_transform();
		s_mat *= scale_image(15.0, 15.0);
		t_mat *= move_image(1.3, -1.2);
		draw_image(tex_hand[1]);


		init_transform();
		s_mat *= scale_image(10.0, 10.0);
		draw_image(tex);
	}

	void CheckCollision() {}

	Gameboy(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//prop//object//gameboy.png", 512, 512, 1);
		set_texture(tex_back[0], "res//prop//object//gameboy_back_black.png", 512, 512, 1);
		set_texture(tex_back[1], "res//prop//object//gameboy_back_white.png", 512, 512, 1);
		set_texture(tex_hand[0], "res//prop//alcy//hand_gameboy_left.png", 512, 512, 1);
		set_texture(tex_hand[1], "res//prop//alcy//hand_gameboy_right.png", 512, 512, 1);
	}

	~Gameboy() {
		glDeleteTextures(1, &tex);
		for (int i = 0; i < 2; ++i) {
			glDeleteTextures(1, &tex_back[i]);
			glDeleteTextures(1, &tex_hand[i]);
		}
	}
};