#pragma once

#include "../../header/HEADER.h"
#include "AlcySprite.h"
#include "CloudSprite.h"

class Game1Controller : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	bool control_state{};

	bool play_state{};

	int score{};

	GLfloat cloud_gen_timer{};

	GLfloat cloud_gen_time = 1500;


public:	
	std::string GetTag() const { return tag; }

	void set_game1_control_state(bool flag) { control_state = flag; }

	bool get_game1_play_state() { return play_state; }

	// ���� ���� �� ���� ����
	void set_game1_play_state(bool flag) { play_state = flag; }


	void game1_key_down(unsigned int key, int x, int y) {
		if (control_state) {
			switch (key) {
			case 32:
				// ���� �÷��� ��
				if (!play_state) {
					play_state = true;
					break;
				}

				// ���� �÷��� ��
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

		// ���� �������� ������ �����Ѵ�
		if (play_state) {
			cloud_gen_timer += fw.FT(1000);

			if (cloud_gen_timer >= cloud_gen_time) {
				fw.AddMainObj(new CloudSprite(main_layer1, "cloud_sprite"), main_layer1);
				cloud_gen_timer = 0;

				std::random_device rd;  std::mt19937 gen(rd());
				std::uniform_int_distribution <int> dis(1500, 3500);
				 // ������ ���� �������� �����Ѵ�
				cloud_gen_time = dis(gen);
			}
		}
	}

	Game1Controller(int l, std::string str) {
		layer = l;
		tag = str;

		// ��Ʈ�ѷ��� �߰��Ǹ� ��������Ʈ�� �߰��Ѵ�
		fw.AddMainObj(new AlcySprite(main_layer2, "alcy_sprite"), main_layer2);
		fw.AddMainObj(new CloudSprite(main_layer1, "cloud_sprite"), main_layer1);
	}
};