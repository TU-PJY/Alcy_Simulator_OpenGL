#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../object/main_object/Cactus.h"

enum game_scene {
	start_up,
	main_screen,
	game1_screen,
	game2_screen,
	score_screen,
};


class Gameboy : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	unsigned int tex{};
	std::array<unsigned int, 2> tex_hand{};
	unsigned int tex_game_logo{};

	bool power_on{};

	GLfloat power_on_delay{};

	GLfloat cactus_timer{};

	int scene = start_up;

public:
	std::string GetTag() const { return tag; }

	void Update() {
		if (!power_on) {
			if (power_on_delay < 1500)
				power_on_delay += fw.FT(1000);

			if (power_on_delay >= 750) {
				cam.target_pos_y = -0.45;
				cam.zoom_value = 1.8;
			}

			if (power_on_delay >= 1500) {
				ssys_game->playSound(boot_sound, 0, false, &ch_game_ef);

				auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "gameboy_back");
				if (ptr) ptr->set_power_on(true);

				power_on = true;
			}
		}

		if (power_on && power_on_delay < 3500) {
			power_on_delay += fw.FT(1000);
			if (power_on_delay >= 3500)
				scene = main_screen;
		}
	}

	void Render() {
		init_transform();
		r_mat *= rotate_image(-15);
		s_mat *= scale_image(15.0, 15.0);
		t_mat *= move_image(-1.3, -1.5);
		draw_image(tex_hand[0]);


		init_transform();
		r_mat *= rotate_image(15);
		s_mat *= scale_image(15.0, 15.0);
		t_mat *= move_image(1.3, -1.5);
		draw_image(tex_hand[1]);

		///////////////

		if (power_on) {
			switch (scene) {
			// logo
			case start_up:
				init_transform();
				t_mat *= move_image(0.0, 0.44);
				s_mat *= scale_image(4.0, 4.0);
				draw_image(tex_game_logo);
				break;

			case main_screen:

				break;
			}
		}


		///////////////

		init_transform();
		s_mat *= scale_image(10.0, 10.0);
		draw_image(tex);
	}

	void CheckCollision() {}

	Gameboy(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//prop//object//gameboy.png", 512, 512, 1);

		set_texture(tex_hand[0], "res//prop//alcy//hand_gameboy_left.png", 512, 512, 1);
		set_texture(tex_hand[1], "res//prop//alcy//hand_gameboy_right.png", 512, 512, 1);

		set_texture(tex_game_logo, "res//prop//object//game_logo.png", 512, 512, 1);
	}

	~Gameboy() {
		glDeleteTextures(1, &tex);

		for (int i = 0; i < 2; ++i)
			glDeleteTextures(1, &tex_hand[i]);
	}
};



class GameboyBack : public MAIN_CLS {
private:
	std::array<unsigned int, 2> tex{};

	int layer{};
	std::string tag{};

	bool power_on{};

public:
	std::string GetTag() const { return tag; }

	void set_power_on(bool flag) {	power_on = flag; }

	void Update() {

	}

	void Render() {
		init_transform();
		s_mat *= scale_image(10.0, 10.0);

		if (power_on)
			draw_image(tex[1]);
		else
			draw_image(tex[0]);
	}

	GameboyBack(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex[0], "res//prop//object//gameboy_back_black.png", 1500, 1500, 1);
		set_texture(tex[1], "res//prop//object//gameboy_back_white.png", 1500, 1500, 1);
	}


	~GameboyBack() {
		for (int i = 0; i < 2; ++i)
			glDeleteTextures(1, &tex[i]);
	}

};