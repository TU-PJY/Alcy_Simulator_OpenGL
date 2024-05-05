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
	void rotate_camera() {
		if (rotate_dir == rotate_right && angle > -10.0)
			angle = std::lerp(angle, -10.0, fw.calc_ft(4));

		else if(rotate_dir == rotate_left && angle < 10.0)
			angle = std::lerp(angle, 10.0, fw.calc_ft(4));

		else if(rotate_dir == rotate_none && angle != 0)
			angle = std::lerp(angle, 0.0, fw.calc_ft(4));
	}

	// ī�޶� �̵�
	void move_camera() {
		x = std::lerp(x, -mx * ratio / 10, fw.calc_ft(15));
		y = std::lerp(y, -my / 10, fw.calc_ft(15));
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


	void update_camera() {
		update_key_state();

		rotate_camera();
		move_camera();
	}
};

extern Camera cam;