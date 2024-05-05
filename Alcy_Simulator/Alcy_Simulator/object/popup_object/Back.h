#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// 메뉴 종료 후 추가하는 배경
class Back2 : public FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex{};
	unsigned int text_tex{};

	int layer{};
	std::string tag{};


	bool delete_flag{};

	HDC hDC{};
	GLuint base{};

	GLfloat transparent = 0.7;
	GLfloat text_transparent = 1.0;

	GLfloat text_x = -1.0 * ratio + 0.05;

public:
	void update() {
		// 완전히 투명해지면 스스로 삭제
		transparent = std::lerp(transparent, 0.0, fw.calc_ft(25));
		text_transparent = std::lerp(text_transparent, 0.0, fw.calc_ft(25));
		text_x = std::lerp(text_x, ratio * -1.0 + 0.5, fw.calc_ft(10));

		if (transparent <= 0.0001)
			delete_flag = true;
	}

	void render() {
		init_transform();
		s_mat *= scale_image(10.0 * ratio, 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex, VAO);


		init_transform();
		set_object_static(text_x / cam.zoom, 0.8 / cam.zoom);
		alpha = text_transparent;

		draw_text(text_tex, VAO, base, "MENU");
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
		set_texture(tex, "res//ui//black.png", 100, 100, 1);

		set_text(text_tex, "white");
		set_font("Somatic Rounded", 80, FW_BOLD, base, hDC);
	}

	~Back2() {
		kill_text(base);
	}
};


// 메뉴 실행 시 추가하는 배경
class Back : public POPUP_FUNCTION {
private:
	GLuint VAO{};
	unsigned int tex{};
	unsigned int text_tex{};

	int layer{};
	std::string tag{};

	HDC hDC{};
	GLuint base{};

	GLfloat transparent{};
	GLfloat text_transparent{};

	GLfloat text_x = ratio * -1.0 + 0.5;


public:
	void update() {
		transparent = std::lerp(transparent, 0.7, fw.calc_ft(10));
		text_transparent = std::lerp(text_transparent, 1.0, fw.calc_ft(10));
		text_x = std::lerp(text_x, -1.0 * ratio + 0.05, fw.calc_ft(10));
	}

	void render() {
		init_transform();
		s_mat *= scale_image(10.0 * ratio, 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex, VAO);


		init_transform();
		set_object_static(text_x / cam.zoom, 0.8 / cam.zoom);
		alpha = text_transparent;

		draw_text(text_tex, VAO, base, "MENU");
	}

	void check_collision() {}

	void check_delete() {}

	Back(int l, std::string str) {
		layer = l;
		tag = str;

		set_canvas(VAO);
		set_texture(tex, "res//ui//black.png", 100, 100, 1);

		set_text(text_tex, "white");
		set_font("Somatic Rounded", 80, FW_BOLD, base, hDC);
	}

	// 메뉴 종료 시 같은 투명 배경을 추가한 후 삭제된다.
	~Back() {
		kill_text(base);
		fw.add_object(new Back2(ui_layer, "back2"), ui_layer);
	}
};