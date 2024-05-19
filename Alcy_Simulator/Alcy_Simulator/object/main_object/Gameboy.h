#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "Game1.h"

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

	int height = HEIGHT - 1;
	Text* text = nullptr;
	GLfloat update_delay{};

	unsigned int tex{};
	std::array<unsigned int, 2> tex_hand{};
	unsigned int tex_game_logo{};

	std::array<unsigned int, 2> tex_game_icon{};
	unsigned int tex_arrow{};


	GLfloat power_on_delay{};
	
	//////
	bool power_on = false;
	int scene = start_up;
	//////

	GLfloat arrow_pos = -0.2;

	bool selector_moved{};

public:
	std::string GetTag() const { return tag; }

	void gameboy_special_key_down(int KEY, int x, int y) {
		switch (KEY) {
		case GLUT_KEY_RIGHT: case GLUT_KEY_LEFT:
			if (scene == main_screen) {
				if (!selector_moved) {
					ssys_game->playSound(selector_sound, 0, false, &ch_game_ef);
					arrow_pos *= -1;
					selector_moved = true;
				}
			}
			break;
		}
	}


	void gameboy_special_key_up(int KEY, int x, int y) {
		switch (KEY) {
		case GLUT_KEY_RIGHT: case GLUT_KEY_LEFT:
			selector_moved = false;
			break;
		}
	}


	void gameboy_key_down(unsigned int key, int x, int y) {
		// game1 컨트롤러로 키보드 입력을 보낸다
		if (scene == game1_screen) {
			auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "game1_controller");
			if (ptr) ptr->game1_key_down(key, x, y);
		}


		switch (key) {
		case 32:
			if (scene == main_screen) {
				if (arrow_pos < 0) {
					scene = game1_screen;

					fw.AddMainObj(new Game1Controller(main_layer1, "game1_controller"), main_layer1);
					auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "game1_controller");
					if (ptr) ptr->set_game1_control_state(true);
				}
				else if (arrow_pos > 0)
					scene = game2_screen;
			}

			break;
		}
	}


	void gameboy_key_up(unsigned int key, int x, int y) {
		// game1 컨트롤러로 키보드 입력을 보낸다
		if (scene == game1_screen) {
			auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "game1_controller");
			if (ptr) ptr->game1_key_up(key, x, y);
		}
	}


	void update_text() {
		if (height != HEIGHT) {
			if (update_delay <= 0) {
				if (text != nullptr)
					delete text;
				text = nullptr;
				text = new Text("Joystix Monospace", 50, FW_DONTCARE);

				height = HEIGHT;
				update_delay = 10;
			}
			else if (update_delay > 0)
				update_delay -= fw.FT(1000);
		}
	}


	void Update() {
		if (!power_on) {
			power_on_delay += fw.FT(1000);

			if (power_on_delay >= 950) {
				cam.target_pos_y = -0.45;
				cam.zoom_value = 1.8;
			}

			if (power_on_delay >= 2000) {
				ssys_game->playSound(boot_sound, 0, false, &ch_game_ef);

				power_on = true;
			}
		}

		else if (power_on) {
			auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "gameboy_back");
			if (ptr && !ptr->get_power_state())
				ptr->set_power_on(true);

			if (scene == start_up) {
				power_on_delay += fw.FT(1000);
				if (power_on_delay >= 4000) {
					ssys_game->playSound(selector_sound, 0, false, &ch_game_ef);
					scene = main_screen;
				}
			}

			else if(scene == main_screen)
				update_text();
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
				s_mat *= scale_image(2.0, 2.0);
				draw_image(tex_game_logo);
				break;

			case main_screen:
				// text
				init_transform();
				if(text != nullptr)
					text->out_static(-0.69, 0.35, 0.235, 0.235, 0.235, "Select Game");

				// game icon
				init_transform();
				t_mat *= move_image(-0.2, 0.45);
				draw_image(tex_game_icon[0]);

				//arrow
				init_transform();
				t_mat *= move_image(arrow_pos, 0.25);
				draw_image(tex_arrow);
				break;
			}

		}

		///////////////
		// gameboy body
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

		set_texture(tex_game_icon[0], "res//prop//object//game1_icon.png", 300, 300, 1);

		set_texture(tex_arrow, "res//prop//object//game_arrow.png", 300, 300, 1);
	}


	~Gameboy() {
		glDeleteTextures(1, &tex);
		glDeleteTextures(1, &tex_game_logo);
		glDeleteTextures(1, &tex_game_icon[0]);
		glDeleteTextures(1, &tex_arrow);

		for (int i = 0; i < 2; ++i)
			glDeleteTextures(1, &tex_hand[i]);

		if(text != nullptr)
			delete text;
		text = nullptr;
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

	bool get_power_state() { return power_on; }

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