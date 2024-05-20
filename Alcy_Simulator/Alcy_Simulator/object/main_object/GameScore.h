#pragma once
#include "../../header/image_text_util.h"


class GameScore : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	int height = HEIGHT;

	Text* text = nullptr;

	int score{};

public:
	std::string GetTag() const { return tag; }

	void Update() {
		if (height != HEIGHT) {
			if (text != nullptr)
				delete text;
			text = nullptr;
			text = new Text("Joystix Monospace", 60, FW_DONTCARE);
			height = HEIGHT;
		}

		auto ptr = fw.FindMainObj_Layer_Single(main_layer1, "game1_controller");
		if (ptr) score = ptr->get_score();
	}

	void Render() {
		init_transform();
		text->out_static(-0.69, 0.34, 0.235, 0.235, 0.235, "%d", score);
	}

	GameScore(int l, std::string str) {
		layer = l;
		tag = str;
		text = new Text("Joystix Monospace", 60, FW_DONTCARE);
	}

	~GameScore() {
		if (text != nullptr)
			delete text;
		text = nullptr;
	}
};