#pragma once

#include "../../header/HEADER.h"
#include "AlcySprite.h"
#include "CloudSprite.h"
#include "Cactus.h"
#include "GameScore.h"

class Game1Controller : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	bool control_state{};

	bool play_state{};

	GLfloat cactus_timer{};
	GLfloat next_cactus_time = 4000;

	double score{};
	double beep_score{};

	GLfloat speed = 0.5;
	bool speed_up_state{};


public:	
	std::string GetTag() const { return tag; }

	void set_game1_control_state(bool flag) { control_state = flag; }

	bool get_game1_play_state() { return play_state; }

	int get_score() { return int(score); }


	void game1_key_down(unsigned int key, int x, int y) {
		if (control_state) {
			switch (key) {
			case 32:
				// 게임 플레이 전
				if (!play_state) {
					play_state = true;
					//ch_bgm->stop();
					//ssys_game->playSound(game_music_1, 0, false, &ch_bgm);
					break;
				}

				// 게임 플레이 중
				else if (play_state) {
					auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_sprite");
					if (ptr) {
						ptr->set_alcy_sprite_jump_enable_state(true);
						ptr->set_alcy_sprite_jump_state(true);
					}
				}
					break;
			}
		}
	}


	void game1_key_up(unsigned int key, int x, int y) {
		if (control_state) {
			switch (key) {
			case 32:
				auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_sprite");
				if (ptr) ptr->set_alcy_sprite_jump_enable_state(false);
				break;
			}
		}
	}

	void Update() {
		if (play_state) {
			cactus_timer += fw.FT(1000);

			if (cactus_timer >= next_cactus_time) {
				std::random_device rd;  std::mt19937 gen(rd());
				std::uniform_int_distribution <int> dis(13, 20);
				next_cactus_time = dis(gen) * 100;

				fw.AddMainObj(new Cactus(main_layer1, "cactus"), main_layer1);
				cactus_timer = 0;
			}


			// level value가 100이 될때마다 알키의 속도를 조금씩 높인다
			score += fw.FT(15 * speed);
			beep_score += fw.FT(15 * speed);

			if (int(beep_score) >= 100) {
				beep_score = 0;

				ch_game_ef2->stop();
				ssys_game->playSound(level_up, 0, false, &ch_game_ef2);
			}

			if (int(score) >= 35 && int(score) < 4000) {
				if (!speed_up_state) {
					speed += 0.3;
					auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_sprite");
					if (ptr) ptr->increase_alcy_sprite_move_speed(speed);
					speed_up_state = true;
				}

				speed += fw.FT(0.005);
				auto ptr = fw.FindMainObj_Layer_Single(main_layer2, "alcy_sprite");
				if (ptr) ptr->increase_alcy_sprite_move_speed(speed);
			}
		}
	}

	Game1Controller(int l, std::string str) {
		layer = l;
		tag = str;

		// 컨트롤러가 추가되면 스프라이트도 추가한다
		fw.AddMainObj(new AlcySprite(main_layer2, "alcy_sprite"), main_layer2);
		fw.AddMainObj(new CloudSprite(main_layer1, "cloud_sprite"), main_layer1);
		fw.AddMainObj(new GameScore(main_layer3, "gamescore"), main_layer3);
	}
};