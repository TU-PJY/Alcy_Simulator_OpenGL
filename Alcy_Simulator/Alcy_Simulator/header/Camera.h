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
	GLfloat zoom = 1.0;

	// 카메라 줌 목표 값
	GLfloat zoom_value = 1.0;



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


	// 카메라 초기화
	void reset_camera() {
		rotate_dir = rotate_none;
		angle = 0;

		x = 0;
		y = 0;

		zoom = 1.0;
		zoom_value = 1.0;

		camera_lock_state = false;

		reset_key_state();
	}


	// 카메라 회전 조작
	void rotate_camera() {
		if (rotate_dir == rotate_right && angle > -8.0)
			angle = std::lerp(angle, -8.0, fw.FT(4));

		else if(rotate_dir == rotate_left && angle < 8.0)
			angle = std::lerp(angle, 8.0, fw.FT(4));

		else if(rotate_dir == rotate_none && angle != 0)
			angle = std::lerp(angle, 0.0, fw.FT(4));
	}



	// 카메라 상하좌우 이동
	void move_camera_xy() {
		x = std::lerp(x, -mx * ratio / 8, fw.FT(15));
		y = std::lerp(y, -my / 8, fw.FT(15));
	}



	// 카메라 줌
	void zoom_camera() {
		zoom = std::lerp(zoom, zoom_value, fw.FT(5));
	}



	// 카메라 잠금 상태 업데이트
	void update_camera_lock() {
		auto ptr = fw.FindMainObj(main_layer7, "cursor_home");

		if (ptr != nullptr) {
			if (ptr->get_touch_state())
				camera_lock_state = true;
			else
				camera_lock_state = false;
		}
	}



	// 카메라 회전 키 누름 상태 업데이트
	void update_key_rotate() {
		if (key_state_right)
			rotate_dir = rotate_right;

		if (key_state_left)
			rotate_dir = rotate_left;

		if ((key_state_right && key_state_left) || (!key_state_right && !key_state_left))
			rotate_dir = rotate_none;
	}



	void update_camera() {
		rotate_camera();
		zoom_camera();
		update_camera_lock();

		// lock state가 true가 되면 카메라 무빙과 조작을 하지 않는다
		if (!camera_lock_state) {
			update_key_rotate();
			move_camera_xy();
		}
	}
};

extern Camera cam;