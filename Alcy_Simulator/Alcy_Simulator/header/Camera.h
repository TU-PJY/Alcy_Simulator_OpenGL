#pragma once
#include "HEADER.h"

enum rotate_dir {
	rotate_left,
	rotate_right,
	rotate_none
};

class Camera {
public:
	// ȸ�� ����
	GLfloat angle{};

	// ī�޶� ��ġ
	GLfloat x{}, y{};

	// ȸ�� ����
	int rotate_dir = rotate_none;

	// Ű ���� ����
	bool key_state_left{}, key_state_right{};

	// ī�޶� ������ ��� ����
	bool camera_lock_state{};


	// Ű ����
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

	// ī�޶� ȸ�� ����
	void rotate_camera_home_mode() {
		if (rotate_dir == rotate_right && angle > -10.0)
			angle = std::lerp(angle, -10.0, fw.calc_ft(4));

		else if(rotate_dir == rotate_left && angle < 10.0)
			angle = std::lerp(angle, 10.0, fw.calc_ft(4));

		else if(rotate_dir == rotate_none && angle != 0)
			angle = std::lerp(angle, 0.0, fw.calc_ft(4));
	}

	// ī�޶� �̵�
	void move_camera_home_mode() {
		x = std::lerp(x, -mx * ratio / 10, fw.calc_ft(15));
		y = std::lerp(y, -my / 10, fw.calc_ft(15));
	}

	// ī�޶� ��� ���� ������Ʈ
	void update_camera_lock() {
		auto ptr = fw.get_ptr(ui_layer, 0);
		if (ptr != nullptr) {
			if (ptr->get_touch_state())
				camera_lock_state = true;
			else
				camera_lock_state = false;
		}
	}

	// Ű ���� ���� ������Ʈ
	void update_key_state() {
		if (key_state_right)
			rotate_dir = rotate_right;

		if (key_state_left)
			rotate_dir = rotate_left;

		if ((key_state_right && key_state_left) || (!key_state_right && !key_state_left))
			rotate_dir = rotate_none;
	}

	//Ȩ ��� ī�޶� ������Ʈ
	void update_home_mode() {
		rotate_camera_home_mode();

		if (!camera_lock_state) {
			update_key_state();
			move_camera_home_mode();
		}
	}

	void update_camera() {
		update_camera_lock();

		if (fw.get_current_mode() == "home_mode") 
			update_home_mode();
	}
};

extern Camera cam;