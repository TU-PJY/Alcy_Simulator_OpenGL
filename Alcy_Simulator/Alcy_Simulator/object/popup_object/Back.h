#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// 메뉴 종료 후 추가하는 배경
class Back2 : public MAIN_CLS {
private:
	unsigned int tex{};

	int layer{};
	std::string tag{};

	bool delete_flag{};

	GLfloat transparent = 0.8;
	GLfloat text_transparent = 1.0;

	GLfloat text_x = rt(-1.0) + 0.1;

	Text* text = nullptr;

	int height = HEIGHT - 1;

	GLfloat update_delay{};

public:
	std::string GetTag() const { return tag; }

	void Update() {
		if (height != HEIGHT) {
			if (update_delay <= 0) {
				if (text != nullptr)
					delete text;
				text = nullptr;
				text = new Text("Maniac", 80, FW_DONTCARE);

				height = HEIGHT;
				update_delay = 10;
			}
			else if(update_delay > 0)
				update_delay -= fw.FT(1000);
		}

		// 완전히 투명해지면 스스로 삭제
		transparent = std::lerp(transparent, 0.0, fw.FT(15));
		text_transparent = std::lerp(text_transparent, 0.0, fw.FT(15));
		text_x = std::lerp(text_x, rt(-1.0) + 0.4, fw.FT(10));

		if (transparent <= 0.0001)
			fw.DeleteMainObj(this, layer);
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(rt(10.0), 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex);


		init_transform();
		alpha = text_transparent;
		text->out_static(text_x, 0.8, 1.0, 1.0, 1.0, "MENU");
	}

	void CheckCollision() {}


	Back2(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//black.png", 100, 100, 1);
	}

	~Back2() {
		if(text != nullptr)
			delete text;
		text = nullptr;

		glDeleteTextures(1, &tex);
	}
};


// 메뉴 실행 시 추가하는 배경
class Back : public SUB_CLS {
private:
	unsigned int tex{};

	int layer{};
	std::string tag{};

	GLfloat transparent{};
	GLfloat text_transparent{};

	GLfloat text_x = rt(-1.0) + 0.4;

	Text* text = nullptr;

	int height = HEIGHT - 1;

	GLfloat update_delay{};

public:
	std::string GetTag() const { return tag; }

	void Update() {
		if (height != HEIGHT) {
			if (update_delay <= 0) {
				if(text != nullptr)
					delete text;
				text = nullptr;
				text = new Text("Maniac", 80, FW_DONTCARE);

				height = HEIGHT;
				update_delay = 10;
			}
			else if (update_delay > 0)
				update_delay -= fw.FT(1000);
		}

		transparent = std::lerp(transparent, 0.8, fw.FT(10));
		text_transparent = std::lerp(text_transparent, 1.0, fw.FT(10));
		text_x = std::lerp(text_x, rt(-1.0) + 0.1, fw.FT(10));
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(rt(10.0), 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex);


		init_transform();
		alpha = text_transparent;
		text->out_static(text_x, 0.8, 1.0, 1.0, 1.0, "MENU");
	}

	void CheckCollision() {}

	Back(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex, "res//ui//black.png", 100, 100, 1);
	}

	// 메뉴 종료 시 같은 투명 배경을 추가한 후 삭제된다.
	~Back() {
		fw.AddMainObj(new Back2(main_layer3, "back2"), main_layer3);
		if (text != nullptr)
			delete text;
		text = nullptr;

		glDeleteTextures(1, &tex);
	}
};