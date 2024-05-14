#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../../header/Camera.h"

// 메뉴 종료 후 추가하는 배경
class Back2 : public MAIN_CLS {
private:
	std::array<unsigned int, 2> tex{};

	int layer{};
	std::string tag{};

	bool delete_flag{};

	GLfloat transparent = 0.7;
	GLfloat text_transparent = 1.0;

	GLfloat text_x = rt(-1.0) + 0.35;

public:
	void Update() {
		// 완전히 투명해지면 스스로 삭제
		transparent = std::lerp(transparent, 0.0, fw.FT(15));
		text_transparent = std::lerp(text_transparent, 0.0, fw.FT(15));
		text_x = std::lerp(text_x, rt(-1.0) + 0.8, fw.FT(10));

		if (transparent <= 0.0001)
			delete_flag = true;
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(rt(10.0), 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex[0]);



		init_transform();
		s_mat *= scale_image(3.0, 3.0);
		set_object_static(dy(text_x),dy(0.8));
		alpha = text_transparent;

		draw_image(tex[1]);
	}

	void CheckCollision() {}


	void CheckDelete() {
		if (delete_flag)
			fw.DeleteMainObj(this, layer);
	}


	Back2(int l, std::string str) {
		layer = l;
		tag = str;

		set_texture(tex[0], "res//ui//black.png", 100, 100, 1);
		set_texture(tex[1], "res//ui//text//text_menu.png", 512, 512, 1);
	}
};


// 메뉴 실행 시 추가하는 배경
class Back : public SUB_CLS {
private:
	GLuint VAO{};
	std::array<unsigned int, 2> tex{};

	int layer{};
	std::string tag{};

	GLfloat transparent{};
	GLfloat text_transparent{};

	GLfloat text_x = ratio * -1.0 + 0.8;



public:
	void Update() {
		transparent = std::lerp(transparent, 0.7, fw.FT(10));
		text_transparent = std::lerp(text_transparent, 1.0, fw.FT(10));
		text_x = std::lerp(text_x, rt(-1.0) + 0.35, fw.FT(10));
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(rt(10.0), 10.0);
		set_object_static(0.0, 0.0);
		alpha = transparent;

		draw_image(tex[0]);


		init_transform();
		s_mat *= scale_image(3.0, 3.0);
		set_object_static(dy(text_x), dy(0.8));
		alpha = text_transparent;

		draw_image(tex[1]);
	}

	void CheckCollision() {}

	void CheckDelete() {}

	Back(int l, std::string str) {
		layer = l;
		tag = str;

		set_vertex(VAO);
		set_texture(tex[0], "res//ui//black.png", 100, 100, 1);
		set_texture(tex[1], "res//ui//text//text_menu.png", 512, 512, 1);
	}

	// 메뉴 종료 시 같은 투명 배경을 추가한 후 삭제된다.
	~Back() {
		fw.AddMainObj(new Back2(main_layer3, "back2"), main_layer3);
	}
};