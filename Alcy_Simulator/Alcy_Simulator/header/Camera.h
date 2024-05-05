#pragma once
#include "HEADER.h"
#include "sound.h"

enum rotate_dir {
	rotate_left,
	rotate_right,
	rotate_none
};

class Camera {
public:
	// 회전 각도
	GLfloat angle{};

	// 카메라 위치
	GLfloat x{}, y{};

	// 회전 방향
	int rotate_dir = rotate_none;

	// 키 누름 상태
	bool key_state_left{}, key_state_right{};

	// 카메라 움직임 잠금 상태
	bool camera_lock_state{};


	// 카메라 줌 결과값
	GLfloat zoom = 1;

	// 카메라 줌 목표 값
	GLfloat zoom_value = 1;


	// 키 조작
	void key_down(unsigned char KEY) {
		switch (KEY) {
		case 'q':
			key_state_left = true;
			break;

		case 'e':
			key_state_right = true;
			break;
		}
	}

	void key_up(unsigned char KEY) {
		switch (KEY) {
		case 'q':
			key_state_left = false;
			break;

		case 'e':
			key_state_right = false;
			break;
		}
	}


	// 키 조작 상태 초기화
	void reset_key_state() {
		key_state_left = false;
		key_state_right = false;
	}


	// 스크롤 조작
	void scroll(int dir) {
		if (dir > 0)
			if (zoom_value <= 2.2) {
				ssys_ui->playSound(scroll_sound, 0, false, &ch_ui);
				zoom_value += 0.2;
			}

		if (dir < 0)
			if (zoom_value >= 0.8) {
				ssys_ui->playSound(scroll_sound, 0, false, &ch_ui);
				zoom_value -= 0.2;
			}
	}

	// 카메라 회전 조작
	void rotate_camera_home_mode() {
		if (rotate_dir == rotate_right && angle > -10.0)
			angle = std::lerp(angle, -10.0, fw.calc_ft(4));

		else if(rotate_dir == rotate_left && angle < 10.0)
			angle = std::lerp(angle, 10.0, fw.calc_ft(4));

		else if(rotate_dir == rotate_none && angle != 0)
			angle = std::lerp(angle, 0.0, fw.calc_ft(4));
	}

	// 카메라 이동
	void move_camera_home_mode() {
		x = std::lerp(x, -mx * ratio / 10, fw.calc_ft(15));
		y = std::lerp(y, -my / 10, fw.calc_ft(15));
	}

	// 카메라 줌
	void zoom_camera_home_mode() {
		zoom = std::lerp(zoom, zoom_value, fw.calc_ft(10));
	}

	// 카메라 잠금 상태 업데이트
	void update_camera_lock() {
		auto ptr = fw.get_ptr(cursor_layer, 0);
		if (ptr != nullptr) {
			if (ptr->get_touch_state())
				camera_lock_state = true;
			else
				camera_lock_state = false;
		}
	}

	// 키 누름 상태 업데이트
	void update_key_state() {
		if (key_state_right)
			rotate_dir = rotate_right;

		if (key_state_left)
			rotate_dir = rotate_left;

		if ((key_state_right && key_state_left) || (!key_state_right && !key_state_left))
			rotate_dir = rotate_none;
	}

	//홈 모드 카메라 업데이트
	void update_home_mode() {
		rotate_camera_home_mode();
		zoom_camera_home_mode();

		if (!camera_lock_state) {
			update_key_state();
			move_camera_home_mode();
		}
	}

	void update_camera() {
		update_camera_lock();
		update_home_mode();
	}
};

extern Camera cam;