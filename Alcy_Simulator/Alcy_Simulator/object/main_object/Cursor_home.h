#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"
#include "../header/Camera.h"


class Cursor_home : public FUNCTION {
private:
	int layer{};
	std::string tag{};

	GLuint VAO{};
	std::array<unsigned int, 3> tex{};
	std::array<const char*, 3> directory = {
		"res//ui//cursor.png",
		"res//ui//cursor_hand.png",
		"res//ui//cursor_finger.png",
	};

	// 커서 위치
	GLfloat x{}, y{};

	// 커서 텍스처 번호
	int tex_number{};

	// 쓰다듬기 상태
	bool touch_state{};

	// 쓰다듬기 시 사용하는 커서 x 위치 및 관련 변수
	GLfloat touch_x{};
	GLfloat touch_number{};


public:
	GLfloat get_cursor_x() const { return x; }
	GLfloat get_cursor_y() const { return y; }
	bool get_touch_state() const { return touch_state; }
	GLfloat get_cursor_touch_x() const { return touch_x; }

	// 커서 타입 업데이트
	void update_cursor_type() {
		// 홈 모드 동작
		auto ptr = fw.get_ptr(alcy_layer, 0);

		if (touch_state) {
			tex_number = 1;
			return;
		}

		if (ptr != nullptr) {
			std::array<GLfloat, 4> touch_zone = ptr->get_touch_zone();

			// 특정 영역에 커서가 위치하면 커서가 바뀐다.
			// 알키 쓰다듬기 영역
			if (ptr->get_intercation_available_state()) {
				if (touch_zone[0] < x && x < touch_zone[1] && touch_zone[2] < y && y < touch_zone[3])
					tex_number = 1;
				else
					tex_number = 0;
			}

			// 커서 - 알키 상호작용 상태가 아니라면 기본 커서를 유지한다
			else
				tex_number = 0;
		}
	}

	// 마우스 왼쪽 버튼 클릭
	void mouse_left_button_down(int button, int state) {
		auto ptr = fw.get_ptr(alcy_layer, 0);

		if (ptr != nullptr) {
			if(ptr->get_intercation_available_state() && tex_number == 1) {
				touch_state = true;
				ptr->tell_touch_state(touch_state);
			}
		}
	}

	// 마우스 왼쪽 버튼 릴리즈
	void mouse_left_button_up(int button, int state) {
		auto ptr = fw.get_ptr(alcy_layer, 0);

		if (ptr != nullptr) {
			touch_state = false;
			ptr->tell_touch_state(touch_state);

			touch_x = 0;
			touch_number = 0;
		}
	}

	// 알키 쓰다듬기 커서 움직임 업데이트
	void move_cursor_touch_state() {
		if (touch_state) {
			touch_number += fw.calc_ft(3);
			touch_x = sin(touch_number) / 4;
		}
	}


	void update() {
		update_cursor_type();
		move_cursor_touch_state();

		if (touch_state) {
			x = touch_x;
			y = 0.2;
		}

		else {
			x = mx * ratio;
			y = my;
		}
	}

	void check_collision() {
		update_cursor_type();
	}

	void render() {
		init_transform();
		set_object_static(x, y);
		draw_image(tex[tex_number], VAO);
	}

	void check_delete() {

	}

	Cursor_home(int l, std::string str) {
		layer = 1;
		tag = str;

		set_canvas(VAO);
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 200, 200, 1);
	}
};