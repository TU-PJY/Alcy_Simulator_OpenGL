#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// 메뉴 종료 후 추가하는 배경
class Back2 : public FUNCTION {
private:
	GLuint VAO{};
	std::array<unsigned int, 2> tex{};

	int layer{};
	std::string tag{};

	bool delete_flag{};

	GLfloat transparent = 0.7;
	GLfloat text_transparent = 1.0;

	GLfloat text_x = -1.0 * ratio + 0.35;

public:
	void update() {
		// 완전히 투명해지면 스스로 삭제
		transparent = std::lerp(transparent, 0.0, fw.calc_ft(15));
		text_transparent = std::lerp(text_transparent, 0.0, fw.calc_ft(15));
		text_x = std::lerp(text_x, ratio * -1.0 + 0.8, fw.calc_ft(10));

		if (transparent <= 0.0001)
			delete_flag = true;
	}

	void render() {
		init_transform();
		s_mat *= scale_image(10.0 * ratio, 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex[0], VAO);



		init_transform();
		s_mat *= scale_image(3.0, 3.0);
		set_object_static(text_x / cam.zoom, 0.8 / cam.zoom);
		alpha = text_transparent;

		draw_image(tex[1], VAO);
	}

	void check_collision() {}


	void check_delete() {
		if (delete_flag)
			fw.delete_object(this, layer);
	}


	Back2(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex[0], "res//ui//black.png", 100, 100, 1);
		set_texture(tex[1], "res//ui//text//text_menu.png", 512, 512, 1);
	}
};


// 메뉴 실행 시 추가하는 배경
class Back : public POPUP_FUNCTION {
private:
	GLuint VAO{};
	std::array<unsigned int, 2> tex{};

	int layer{};
	std::string tag{};

	GLfloat transparent{};
	GLfloat text_transparent{};

	GLfloat text_x = ratio * -1.0 + 0.8;



public:
	void update() {
		transparent = std::lerp(transparent, 0.7, fw.calc_ft(10));
		text_transparent = std::lerp(text_transparent, 1.0, fw.calc_ft(10));
		text_x = std::lerp(text_x, -1.0 * ratio + 0.35, fw.calc_ft(10));
	}

	void render() {
		init_transform();
		s_mat *= scale_image(10.0 * ratio, 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex[0], VAO);


		init_transform();
		s_mat *= scale_image(3.0, 3.0);
		set_object_static(text_x / cam.zoom, 0.8 / cam.zoom);
		alpha = text_transparent;

		draw_image(tex[1], VAO);
	}

	void check_collision() {}

	void check_delete() {}

	Back(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex[0], "res//ui//black.png", 100, 100, 1);
		set_texture(tex[1], "res//ui//text//text_menu.png", 512, 512, 1);
	}

	// 메뉴 종료 시 같은 투명 배경을 추가한 후 삭제된다.
	~Back() {
		fw.add_object(new Back2(layer3, "back2"), layer3);
	}
};