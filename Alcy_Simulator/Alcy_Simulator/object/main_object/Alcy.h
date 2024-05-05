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

	// �Ӹ� ���ϴ� ����
	int head_state = head_middle;

	// �Ӹ� ȸ�� ����
	int head_rotate_state = head_rotate_none;

	// �Ӹ� ��ġ
	GLfloat head_position{};

	// �Ӹ� ȸ�� ����
	GLfloat head_angle{};


	// �� ������ ����
	bool blink_state{};

	// �� ������ ���� ����
	time_t start_time, end_time{};

	// �� ���� ���� �����ϴ� �ð�
	GLfloat blink_time{};

	// �� ������ ����
	GLfloat blink_interval{};

public:
	// �Ӹ� ���� ������Ʈ
	void update_head_dir() {
		// ī�޶� ȸ�� ���߿��� ���� �ٶ󺻴�
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

	// �Ӹ� ȸ�� ������Ʈ
	void update_rotate_head() {
		if (head_rotate_state == head_rotate_right)
			head_angle = std::lerp(head_angle, -10.0, fw.calc_ft(4));

		else if (head_rotate_state == head_rotate_left)
			head_angle = std::lerp(head_angle, 10.0, fw.calc_ft(4));

		if (head_rotate_state == head_rotate_none)
			head_angle = std::lerp(head_angle, 0.0, fw.calc_ft(4));
	}


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


	void update() {
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