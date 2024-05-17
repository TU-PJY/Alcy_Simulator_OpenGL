#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"

class Gameboy : public MAIN_CLS {
private:
	int layer{};
	std::string tag{};

	unsigned int tex{};

public:
	std::string GetTag() const { return tag; }

	void Update() {}

	void Render() {
		init_transform();
		draw_image(tex);
	}

	void CheckCollision() {}
	void CheckDelete(){}

	Gameboy(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//prop//object//gameboy.png", 1500, 1500, 1);
	}
};