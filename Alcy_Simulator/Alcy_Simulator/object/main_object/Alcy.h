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


	// 눈 깜빡임 여부
	bool blink_state{};

	// 눈 깜빡임 시작 측정
	time_t start_time, end_time{};

	// 눈 감은 상태 유지하는 시간
	GLfloat blink_time{};

	// 눈 깜빡임 간격
	GLfloat blink_interval{};

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


	void update_blink() {  // 알키 눈 깜빡임 업데이트
		std::random_device rd;  std::mt19937 gen(rd());
		std::uniform_real_distribution <GLfloat> dis(0, 3);

		if (!blink_state) {
			end_time = time(NULL);  // 시간을 실시간으로 측정하여 아래의 조건에 맞는지 검사한다
			if (GLfloat(end_time - start_time) > blink_interval)  // 지정된 간격보다 시간이 지나면 눈 깜빡임이 활성화 된다.
				blink_state = true;
		}

		else {
			blink_time += fw.calc_ft(1);  // 아주 짧은 시간 동안 눈 감은 상태를 유지하다가 다시 눈을 뜬다.

			if (blink_time > 0.2) {
				start_time = time(NULL);
				blink_time = 0;
				blink_interval = dis(gen);  // 눈 깜빡이는 간격을 랜덤으로 설정한다
				blink_state = false;
			}
		}
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

		update_blink();
		face.tell_blink_state(blink_state);
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