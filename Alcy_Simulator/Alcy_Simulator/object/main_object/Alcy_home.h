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


	// Ȩ ��� ���� �� ��� �Ŀ� ȯ�� ���带 ����Ѵ�
	GLfloat welcome_timer{};

	// ȯ�� �޽��� ��� ����
	bool welcome_state{};

	// �Ӹ� ���ϴ� ����
	int head_state = head_middle;

	// �Ӹ� ȸ�� ����
	int head_rotate_state = head_rotate_none;
	int prev_rotate_state = head_rotate_none;

	// �Ӹ� ��ġ
	GLfloat head_position{};

	// �Ӹ� ȸ�� ����
	GLfloat head_angle{};



	// �� ������ ����
	bool blink_state{};

	// �� ������ ���� ����
	time_t start_time{}, end_time{};

	// �� ���� ���� �����ϴ� �ð�
	GLfloat blink_time{};

	// �� ������ ����
	GLfloat blink_interval{};



	// ��ȣ �ۿ� ���� ����
	bool interaction_available_state{};



	// �Ӹ� ���ٵ�� Ȱ��ȭ ����
	// x min, x max, y min, y max
	std::array<GLfloat, 4> touch_zone = {
		-0.15, 0.15, 0.25, 0.7
	};

	// �Ӹ� ���ٵ�� ����
	bool touch_state{};

	// �Ӹ� ���ٵ�� ����
	GLfloat touch_angle{};


	// �� ������ Ȱ��ȭ ����
	std::array<GLfloat, 4> squeak_zone = {
		-0.05, 0.05, -0.1, 0.0
	};

	// �� ���� ����
	bool squeak_state{};

	// �� ���� ���� ���� �ð�
	GLfloat squeak_time{};


public:
	// ���ٵ�� Ȱ��ȭ ���� ����
	std::array<GLfloat, 4> get_touch_zone() const { return touch_zone; }
	
	// �� ������ Ȱ��ȭ ���� ����
	std::array<GLfloat, 4> get_squeak_zone() const { return squeak_zone; }

	// Ŀ�� - ��Ű ��ȣ�ۿ� ���� ���� ����
	bool get_interaction_available_state() const { return interaction_available_state; }


	// ���ٵ�� ���� �˸�
	void tell_touch_state(bool state) { 
		touch_state = state; 
		
		// ���ٵ�� �Ҹ� ���
		if (touch_state)
			ssys->playSound(touch, 0, false, &ch_alcy);
		else 
			ch_alcy->stop();
	}

	// �� ������ ���� �˸�
	void tell_squeak_state(bool state) { 
		squeak_state = state; 

		// ���� ���� ���
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


	// �Ӹ� ���� ������Ʈ
	void update_head_dir() {
		// ī�޶� ȸ�� ���߿��� ���� �ٶ󺻴�
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

	// �Ӹ� ��ġ ������Ʈ
	void update_move_head() {
		if (head_state == head_right)
			head_position = std::lerp(head_position, 0.1, fw.calc_ft(6));

		else if (head_state == head_left)
			head_position = std::lerp(head_position, -0.1, fw.calc_ft(6));

		else if (head_state == head_middle)
			head_position = std::lerp(head_position, 0.0, fw.calc_ft(6));
	}

	// �Ӹ� ȸ�� ���� ������Ʈ
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

	// �Ӹ� ȸ�� ������Ʈ
	void update_rotate_head() {
		if (head_rotate_state == head_rotate_right)
			head_angle = std::lerp(head_angle, -10.0, fw.calc_ft(3));

		else if (head_rotate_state == head_rotate_left)
			head_angle = std::lerp(head_angle, 10.0, fw.calc_ft(3));

		if (head_rotate_state == head_rotate_none)
			head_angle = std::lerp(head_angle, 0.0, fw.calc_ft(3));
	}

	// �� ������ ������Ʈ
	void update_blink() {  // ��Ű �� ������ ������Ʈ
		std::random_device rd;  std::mt19937 gen(rd());
		std::uniform_real_distribution <GLfloat> dis(0, 3);

		if (!blink_state) {
			end_time = time(NULL);  // �ð��� �ǽð����� �����Ͽ� �Ʒ��� ���ǿ� �´��� �˻��Ѵ�
			if (GLfloat(end_time - start_time) > blink_interval)  // ������ ���ݺ��� �ð��� ������ �� �������� Ȱ��ȭ �ȴ�.
				blink_state = true;
		}

		else {
			blink_time += fw.calc_ft(1);  // ���� ª�� �ð� ���� �� ���� ���¸� �����ϴٰ� �ٽ� ���� ���.

			if (blink_time > 0.2) {
				start_time = time(NULL);
				blink_time = 0;
				blink_interval = dis(gen);  // �� �����̴� ������ �������� �����Ѵ�
				blink_state = false;
			}
		}
	}

	// Ŀ�� - ��Ű ��ȣ�ۿ� ���� ���� ������Ʈ
	void update_intercation_state() {
		if (head_rotate_state == head_rotate_none && cam.angle < 2.0 && cam.angle > -2.0 && !squeak_state)
			interaction_available_state = true;

		else
			interaction_available_state = false;
	}

	// ��Ű ���ٵ�� ������Ʈ
	void update_touch_state() {
		// ���ٵ�� ���ȿ��� ���� ��� ������ �Ѵ�
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

		// ���ٵ�⸦ ���߸� �Ӹ� ������ �ٽ� �ǵ�����
		else {
			touch_angle = std::lerp(touch_angle, 0, fw.calc_ft(10));

			face.tell_touch_angle(touch_angle);
			head.tell_touch_angle(touch_angle);
			tail.tell_touch_angle(touch_angle);
			hair.tell_touch_angle(touch_angle);
		}
	}

	// ��Ű �� ���� ������Ʈ
	void update_squeak_state() {  // �� ������
		if (squeak_state) {  // �����ð����� ��Ű�� �ڱ� �ڸ� �ٶ󺻴�
			face.enable_state_static();
			face.tell_squeak_state(squeak_state);
			squeak_time += fw.calc_ft(1);

			if (squeak_time > 1.5) {  // 1.5�� �� ����
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
		// ȯ�� �޽��� Ÿ�̸� ������Ʈ
		update_welcome_timer();

		//�Ӹ� ���� ������Ʈ
		update_head_dir();

		// �Ӹ� ��ġ ������Ʈ
		update_move_head();

		// �Ӹ� ȸ�� ���� ������Ʈ
		update_head_rotate_state();

		// �Ӹ� ȸ�� ������Ʈ
		update_rotate_head();

		// �Ӹ� ����
		tail.tell_head_angle(head_angle);
		hair.tell_head_angle(head_angle);
		head.tell_head_angle(head_angle);
		face.tell_head_angle(head_angle);

		// �Ӹ� ����
		head.tell_head_state(head_state);
		face.tell_head_state(head_state);

		// �Ӹ� ��ġ
		head.tell_head_position(head_position);
		face.tell_head_position(head_position);

		// �� ������
		update_blink();
		face.tell_blink_state(blink_state);

		// Ŀ�� ��ȣ�ۿ� ���� ���� ������Ʈ
		update_intercation_state();

		// ���ٵ�� ���� ������Ʈ
		update_touch_state();

		// �� ������ ���� ������Ʈ
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