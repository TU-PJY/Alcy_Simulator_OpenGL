#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"


class AlcySprite : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	std::array<unsigned int, 3> tex{};
	std::array<const char*, 3> directory = {
		"res//prop//object//alcy_sprite_1.png",
		"res//prop//object//alcy_sprite_2.png",
		"res//prop//object//alcy_sprite_jump.png",
	};

	bool move_state{};

	bool jump_state{};

	bool jump_enable_state{};

	bool jump_sound_played{};

	GLfloat frame = 1;

	GLfloat height = 0.3;

	GLfloat acc = 1.2;

	GLfloat move_speed = 0.5;

public:

	std::string GetTag() const { return tag; }

	void set_alcy_sprite_jump_state(bool flag) { jump_state = flag; }

	void set_alcy_sprite_jump_enable_state(bool flag) { jump_enable_state = flag; }

	GLfloat get_alcy_sprite_move_speed() { return move_speed; }

	void increase_alcy_sprite_move_speed(GLfloat speed) { move_speed = speed; }


	void Update() {
		auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "game1_controller");

		if (ptr && ptr->get_game1_play_state()) {
			frame += fw.FT(8);

			if (jump_state) {
				//한 번에 한 번만 출력하도록 한다
				if (!jump_sound_played) {
					ch_game_ef->stop();
					ssys_game->playSound(jump_sound, 0, false, &ch_game_ef);
					jump_sound_played = true;
				}

				height += fw.FT(acc);
				acc -= fw.FT(4);

				if (acc < 0 && height <= 0.3) {
					jump_sound_played = false;

					height = 0.3;
					acc = 1.2;

					if (!jump_enable_state) 
						jump_state = false;
				}
			}
		}
	}


	void CheckCollision() {

	}


	void Render() {
		init_transform();

		t_mat *= move_image(-0.33, height);
		s_mat *= scale_image(0.8, 0.8);

		if(jump_state)
			draw_image(tex[2]);
		else
			draw_image(tex[int(frame) % 2]);
	}


	AlcySprite(int l, std::string str) {
		layer = l;
		tag = str;

		for (int i = 0; i < 3; ++i)
			set_texture(tex[i], directory[i], 300, 300, 1);
	}

	~AlcySprite() {
		for (int i = 0; i < 3; ++i)
			glDeleteTextures(1, &tex[i]);
	}
};