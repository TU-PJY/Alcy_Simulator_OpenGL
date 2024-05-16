#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"


class Zoom_ind : public MAIN_CLS {
private:
	unsigned int tex{};
	GLfloat transparent{};

	int layer{};
	std::string tag{};

	int count = 3;

	Text* text;

	int height = HEIGHT;


public:
	std::string GetTag() const { return tag; }

	void Update() {
		if (height != HEIGHT) {
			delete text;
			text = new Text("Maniac", 100, FW_DONTCARE);

			height = HEIGHT;
		}

		if (count != cam.zoom_count) {
			transparent = 10.0;
			count = cam.zoom_count;
		}

		transparent = std::lerp(transparent, 0.0, fw.FT(5));
	}

	void Render() {
		init_transform();
		alpha = transparent;
		if(cam.zoom_count - 3 >= 0)
			text->out_static(rt(-1.0) + 0.1, -0.9, 1.0, 1.0, 1.0, "Zoom x%d", cam.zoom_count - 3);

		else
			text->out_static(rt(-1.0) + 0.1, -0.9, 1.0, 1.0, 1.0, "Zoom -x%d", -1 * (cam.zoom_count - 3));
	}

	void CheckCollision(){}
	void CheckDelete(){}

	Zoom_ind(int l, std::string str) {
		layer = l;
		tag = str;
		text = new Text("Maniac", 100, FW_DONTCARE);
	}

	~Zoom_ind() {
		delete text;
	}
};