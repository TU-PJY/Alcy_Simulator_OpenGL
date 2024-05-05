#pragma once
#include "../../header/Camera.h"
#include "Alcy_parts.h"


class Alcy : public FUNCTION {
private:
	int layer{};
	std::string tag{};

	Head head;
	Body body;
	Face face;
	Hair hair;
	Tail tail;

	// 머리 향하는 방향
	int head_state = head_middle;

	// 머리 회전 방향
	int head_rotate_state = head_rotate_none;

	// 머리 위치
	GLfloat head_position{};

	// 머리 회전 각도
	GLfloat head_angle{};

public:
	// 머리 방향 업데이트
	void update_head_dir() {
		// 카메라 회전 도중에는 앞을 바라본다
		if (cam.key_state_left || cam.key_state_right || head_rotate_state != head_rotate_none)
			head_state = head_middle;

		else {
			if (mx > 0.5)
				head_state = head_right;

			else if (mx < -0.5)
				head_state = head_left;

			else
				head_state = head_middle;
		}
	}

	// 머리 위치 업데이트
	void update_move_head() {
		if (head_state == head_right)
			head_position = std::lerp(head_position, 0.1, fw.calc_ft(6));

		else if (head_state == head_left)
			head_position = std::lerp(head_position, -0.1, fw.calc_ft(6));

		else if (head_state == head_middle)
			head_position = std::lerp(head_position, 0.0, fw.calc_ft(6));
	}

	// 머리 회전 상태 업데이트
	void update_head_rotate_state() {
		if (cam.key_state_left || cam.key_state_right || head_rotate_state != head_rotate_none)
			face.enable_state_static();

		if (cam.key_state_right && !cam.key_state_left && cam.angle < -7.0)
			head_rotate_state = head_rotate_right;
		
		else if(!cam.key_state_right && cam.key_state_left && cam.angle > 7.0)
			head_rotate_state = head_rotate_left;

		else if (!cam.key_state_right && !cam.key_state_left && cam.angle < 2.0 && cam.angle > -2.0) {
			head_rotate_state = head_rotate_none;
			face.unable_state_static();
		}
	}

	// 머리 회전 업데이트
	void update_rotate_head() {
		if (head_rotate_state == head_rotate_right)
			head_angle = std::lerp(head_angle, -10.0, fw.calc_ft(4));

		else if (head_rotate_state == head_rotate_left)
			head_angle = std::lerp(head_angle, 10.0, fw.calc_ft(4));

		if (head_rotate_state == head_rotate_none)
			head_angle = std::lerp(head_angle, 0.0, fw.calc_ft(4));
	}


	void update() {
		//머리 방향 업데이트
		update_head_dir();

		// 머리 위치 업데이트
		update_move_head();

		// 머리 회전 상태 업데이트
		update_head_rotate_state();

		// 머리 회전 업데이트
		update_rotate_head();

		// 머리 각도
		tail.tell_head_angle(head_angle);
		hair.tell_head_angle(head_angle);
		head.tell_head_angle(head_angle);
		face.tell_head_angle(head_angle);

		// 머리 방향
		head.tell_head_state(head_state);
		face.tell_head_state(head_state);

		// 머리 위치
		head.tell_head_position(head_position);
		face.tell_head_position(head_position);
	}

	void check_collision() {

	}

	void render() {
		tail.render();
		body.render();
		hair.render();
		head.render();
		face.render();
	}


	void check_delete() {

	}

	Alcy(int l, std::string str) {
		layer = l;
		tag = str;
	}
};