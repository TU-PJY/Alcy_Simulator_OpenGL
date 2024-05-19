#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"

class CloudSprite : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	unsigned int tex{};

	GLfloat x{}, y{};

	bool move_state{};

	GLfloat move_speed{};

	bool clone_state{};


public:
	std::string GetTag() const { return tag; }

	void set_cloud_sprite_move_state(bool flag) { move_state = flag; }

	void Update() {
		auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "game1_controller");

		if (ptr && ptr->get_game1_play_state()) {
			auto ptr2 = fw.FindMainObj_Layer_Single(main_layer2, "alcy_sprite");
			if (ptr2) move_speed = ptr2->get_alcy_sprite_move_speed();

			x -= fw.FT(move_speed / 2);

			// 자신의 위치가 -0.4 가 되면 새로운 구름을 생성한다
			if (x <= -0.2 && !clone_state) {
				fw.AddMainObj(new CloudSprite(main_layer1, "cloud_sprite"), main_layer1);
				clone_state = true;
			}

			if (x <= -0.6)
				fw.DeleteMainObj(this, layer);
		}
		
	}

	void Render() {
		init_transform();
		t_mat *= move_image(x, y);
		draw_image(tex);
	}

	CloudSprite(int l, std::string str) {
		layer = l;
		tag = str;

		std::random_device rd;  std::mt19937 gen(rd());
		std::uniform_real_distribution <GLfloat> dis(0.45, 0.65);

		GLfloat height = dis(gen);

		y = height;
		x = 0.6;

		set_texture(tex, "res//prop//object//cloud_sprite.png", 130, 130, 1);
	}

	~CloudSprite() {
		glDeleteTextures(1, &tex);
	}
};