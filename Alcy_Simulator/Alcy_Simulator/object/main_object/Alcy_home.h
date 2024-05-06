#pragma once
#include "../../header/Camera.h"
#include "Alcy_home_parts.h"
#include "../../header/sound.h"
#include "Welcome_messege.h"


class Alcy_home : public FUNCTION {
private:
	int layer{};
	std::string tag{};

	Head head;
	Body body;
	Face face;
	Hair hair;
	Tail tail;


	// 홈 모드 진입 후 잠시 후에 환영 사운드를 출력한다
	GLfloat welcome_timer{};

	// 환영 메시지 출력 여부
	bool welcome_state{};

	// 머리 향하는 방향
	int head_state = head_middle;

	// 머리 회전 방향
	int head_rotate_state = head_rotate_none;
	int prev_rotate_state = head_rotate_none;

	// 머리 위치
	GLfloat head_position{};

	// 머리 회전 각도
	GLfloat head_angle{};



	// 눈 깜빡임 여부
	bool blink_state{};

	// 눈 깜빡임 시작 측정
	time_t start_time{}, end_time{};

	// 눈 감은 상태 유지하는 시간
	GLfloat blink_time{};

	// 눈 깜빡임 간격
	GLfloat blink_interval{};



	// 상호 작용 가능 상태
	bool interaction_available_state{};



	// 머리 쓰다듬기 활성화 영역
	// x min, x max, y min, y max
	std::array<GLfloat, 4> touch_zone = {
		-0.15, 0.15, 0.25, 0.7
	};

	// 머리 쓰다듬기 상태
	bool touch_state{};

	// 머리 쓰다듬기 각도
	GLfloat touch_angle{};


	// 코 누르기 활성화 영역
	std::array<GLfloat, 4> squeak_zone = {
		-0.05, 0.05, -0.1, 0.0
	};

	// 코 눌림 상태
	bool squeak_state{};

	// 코 눌림 상태 유지 시간
	GLfloat squeak_time{};


public:
	// 쓰다듬기 활성화 영역 리턴
	std::array<GLfloat, 4> get_touch_zone() const { return touch_zone; }
	
	// 코 누르기 활성화 영역 리턴
	std::array<GLfloat, 4> get_squeak_zone() const { return squeak_zone; }

	// 커서 - 알키 상호작용 가능 상태 리턴
	bool get_interaction_available_state() const { return interaction_available_state; }


	// 쓰다듬기 상태 알림
	void tell_touch_state(bool state) { 
		touch_state = state; 
		
		// 쓰다듬기 소리 출력
		if (touch_state)
			ssys->playSound(touch, 0, false, &ch_alcy);
		else 
			ch_alcy->stop();
	}

	// 코 누르기 상태 알림
	void tell_squeak_state(bool state) { 
		squeak_state = state; 

		// 랜덤 사운드 재생
		std::random_device rd;  std::mt19937 gen(rd());
		std::uniform_int_distribution <int> dis(1, 3);

		int number = dis(gen);
		switch (number) {
		case 1:
			ssys->playSound(squeak1, 0, false, &ch_alcy); break;
		case 2:
			ssys->playSound(squeak2, 0, false, &ch_alcy); break;
		case 3:
			ssys->playSound(squeak3, 0, false, &ch_alcy); break;
		}
	}


	// 머리 방향 업데이트
	void update_head_dir() {
		// 카메라 회전 도중에는 앞을 바라본다
		if (cam.key_state_left || cam.key_state_right || head_rotate_state != head_rotate_none || touch_state || squeak_state)
			head_state = head_middle;

		else {
			if (mx > 0.55)
				head_state = head_right;

			else if (mx < -0.55)
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
		if (!squeak_state) {

			if (cam.key_state_left || cam.key_state_right || head_rotate_state != head_rotate_none)
				face.enable_state_static();

			if (cam.key_state_right && !cam.key_state_left && cam.angle < -7.0)
				head_rotate_state = head_rotate_right;

			else if (!cam.key_state_right && cam.key_state_left && cam.angle > 7.0)
				head_rotate_state = head_rotate_left;

			else if (!cam.key_state_right && !cam.key_state_left && cam.angle < 2.0 && cam.angle > -2.0) {
				head_rotate_state = head_rotate_none;
				face.unable_state_static();
			}

			if (prev_rotate_state != head_rotate_state) {
				prev_rotate_state = head_rotate_state;
				ssys->playSound(tilt, 0, false, &ch_alcy);
			}
		}
	}

	// 머리 회전 업데이트
	void update_rotate_head() {
		if (head_rotate_state == head_rotate_right)
			head_angle = std::lerp(head_angle, -10.0, fw.calc_ft(3));

		else if (head_rotate_state == head_rotate_left)
			head_angle = std::lerp(head_angle, 10.0, fw.calc_ft(3));

		if (head_rotate_state == head_rotate_none)
			head_angle = std::lerp(head_angle, 0.0, fw.calc_ft(3));
	}

	// 눈 깜빡임 업데이트
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

	// 커서 - 알키 상호작용 가능 상태 업데이트
	void update_intercation_state() {
		if (head_rotate_state == head_rotate_none && cam.angle < 2.0 && cam.angle > -2.0 && !squeak_state)
			interaction_available_state = true;

		else
			interaction_available_state = false;
	}

	// 알키 쓰다듬기 업데이트
	void update_touch_state() {
		// 쓰다듬는 동안에는 눈을 계속 감도록 한다
		if (touch_state) {
			face.enable_state_static();
			face.tell_blink_state(true);

			auto ptr = fw.get_ptr(cursor_layer, 0);
			if (ptr != nullptr) {
				touch_angle = -ptr->get_cursor_touch_x() * 20;

				face.tell_touch_angle(touch_angle);
				head.tell_touch_angle(touch_angle);
				tail.tell_touch_angle(touch_angle);
				hair.tell_touch_angle(touch_angle);
			}
		}

		// 쓰다듬기를 멈추면 머리 각도를 다시 되돌린다
		else {
			touch_angle = std::lerp(touch_angle, 0, fw.calc_ft(10));

			face.tell_touch_angle(touch_angle);
			head.tell_touch_angle(touch_angle);
			tail.tell_touch_angle(touch_angle);
			hair.tell_touch_angle(touch_angle);
		}
	}

	// 알키 코 누름 업데이트
	void update_squeak_state() {  // 코 누르기
		if (squeak_state) {  // 일정시간동안 알키는 자기 코를 바라본다
			face.enable_state_static();
			face.tell_squeak_state(squeak_state);
			squeak_time += fw.calc_ft(1);

			if (squeak_time > 1.5) {  // 1.5초 후 해제
				squeak_state = false;
				face.tell_squeak_state(squeak_state);
				squeak_time = 0;
			}
		}
	}

	void update_welcome_timer() {
		if (!welcome_state) {
			welcome_timer += fw.calc_ft(1);
			if (welcome_timer > 1.0) {
				fw.add_object(new Welcome_messege(prop_layer2, "welcome_messege"), prop_layer2);
				welcome_state = true;
			}
		}
	}


	void update() {
		// 환영 메시지 타이머 업데이트
		update_welcome_timer();

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

		// 눈 깜빡임
		update_blink();
		face.tell_blink_state(blink_state);

		// 커서 상호작용 가능 여부 업데이트
		update_intercation_state();

		// 쓰다듬기 상태 업데이트
		update_touch_state();

		// 코 누르기 상태 업데이트
		update_squeak_state();
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

	Alcy_home(int l, std::string str) {
		layer = l;
		tag = str;
	}
};