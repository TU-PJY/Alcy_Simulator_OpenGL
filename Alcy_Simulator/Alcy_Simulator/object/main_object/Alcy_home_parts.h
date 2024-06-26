#pragma once
#include "../../header/image_text_util.h"
#include "../../header/view.h"

// 머리 방향 상태
enum head_state {
	head_left,
	head_middle,
	head_right
};

// 머리 회전 방향 상태
enum head_rotate {
	head_rotate_right,
	head_rotate_left,
	head_rotate_none
};



class Face {
private:
	std::array<unsigned int, 3> tex_eye{};
	std::array<unsigned int, 3> tex_blink{};
	std::array<unsigned int, 3> tex_dot{};
	std::array<unsigned int, 3> tex_brow{};
	unsigned int tex_eye_squeak{};

	std::array<const char*, 3> directory_eye = {
		"res//alcy//face//eye_left.png",
		"res//alcy//face//eye_middle.png",
		"res//alcy//face//eye_right.png",
	};

	std::array<const char*, 3> directory_blink = {
		"res//alcy//face//blink_left.png",
		"res//alcy//face//blink_middle.png",
		"res//alcy//face//blink_right.png",
	};

	std::array<const char*, 3> directory_dot = {
		"res//alcy//face//dot_left.png",
		"res//alcy//face//dot_middle.png",
		"res//alcy//face//dot_right.png",
	};

	std::array<const char*, 3> directory_brow = {
		"res//alcy//face//brow_left.png",
		"res//alcy//face//brow_middle.png",
		"res//alcy//face//brow_right.png",
	};

	const char* directory_eye_squeak = "res//alcy//face//eye_squeak.png";

	// 눈 깜빡임 상태, true일 시 blink 출력, false일 시 eye 출력
	bool blink_state{};

	GLfloat position{};
	GLfloat tilt_angle{};

	GLfloat touch_angle{};

	// 머리 방향
	int head_state{};
	
	// 머리 회전 여부
	bool head_rotate_state{};

	// 코 눌림 여부
	bool squeak_state{};


public:
	void tell_head_state(int state) { head_state = state; }

	// 눈 깜빡임 상태 알리기
	void tell_blink_state(bool state) { blink_state = state; }

	// 머리 위치 알리기
	void tell_head_position(GLfloat pos) { position = pos; }

	// 머리 회전 각도 알리기
	void tell_head_angle(GLfloat a) { tilt_angle = a; }

	// 쓰다듬기 머리 회전 각도 알리기
	void tell_touch_angle(GLfloat a) { touch_angle = a; }

	// 코 누름 상태 알리기
	void tell_squeak_state(bool state) { squeak_state = state; }

	// 머리 회전 여부 활성화 / 비활성화
	void enable_state_static() {
		head_rotate_state = true;
	}
	void unable_state_static() {
		head_rotate_state = false;
	}

	// 눈
	void render_eye() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.2);

		
		GLfloat x = mx / 45;
		GLfloat y = my / 45;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.7);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.7);

		if(!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_eye[head_state]);
	}

	// 코 누름 상태의 눈
	void render_eye_squeak() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.2);

		GLfloat x = mx / 50;
		GLfloat y = my / 50;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.7);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.7);

		if (!head_rotate_state)
			t_mat *= move_image(x, y);

		draw_image(tex_eye_squeak);
	}

	// 눈 깜빡임
	void render_blink() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.2);

		GLfloat x = mx / 50;
		GLfloat y = my / 50;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.7);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.7);

		if (!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_blink[head_state]);
	}

	// 눈동자
	void render_dot() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.2);

		GLfloat x{};
		GLfloat y{};

		if (head_state == 1) {
			x = mx / 18;
			y = my / 18;
		}

		else if (head_state == 0 || head_state == 2) {
			x = mx / 23;
			y = my / 23;
		}

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.7);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.7);
		
		if (!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_dot[head_state]);
	}
	
	// 눈썹
	void render_brow() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.2);

		GLfloat x = mx / 50;
		GLfloat y = my / 50;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.7);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.7);

		if (!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_brow[head_state]);
	}


	void Render() {
		// blink state가 true이면 눈을 깜빡이는 것
		if (!blink_state) {
			if (!squeak_state) {
				render_eye();
				render_dot();
			}
			else
				render_eye_squeak();
		}

		else
			render_blink();

		render_brow();
	}

	Face() {
		for (int i = 0; i < directory_eye.size(); ++i) 
			set_texture(tex_eye[i], directory_eye[i], 1500, 1500, 1);

		for (int i = 0; i < directory_blink.size(); ++i)
			set_texture(tex_blink[i], directory_blink[i], 1500, 1500, 1);

		for (int i = 0; i < directory_dot.size(); ++i)
			set_texture(tex_dot[i], directory_dot[i], 1500, 1500, 1);

		for (int i = 0; i < directory_brow.size(); ++i)
			set_texture(tex_brow[i], directory_brow[i], 1500, 1500, 1);

		set_texture(tex_eye_squeak, directory_eye_squeak, 1500, 1500, 1);
	}

	~Face() {
		for (int i = 0; i < 3; ++i) {
			glDeleteTextures(1, &tex_eye[i]);
			glDeleteTextures(1, &tex_dot[i]);
			glDeleteTextures(1, &tex_brow[i]);
			glDeleteTextures(1, &tex_blink[i]);
		}
	}
};



class Head {
private:
	std::array <unsigned int, 3> tex{};
	std::array<const char*, 3> directory = {
		"res//alcy//head_left.png",
		"res//alcy//head_middle.png",
		"res//alcy//head_right.png",
	};

	GLfloat position{};
	GLfloat tilt_angle{};

	GLfloat touch_angle{};

	// 머리 방향
	int head_state{};
	

public:
	void tell_head_state(int state) {
		head_state = state;
	}

	void tell_head_position(GLfloat pos) {
		position = pos;
	}

	void tell_head_angle(GLfloat a) {
		tilt_angle = a;
	}

	void tell_touch_angle(GLfloat a) {
		touch_angle = a;
	}


	void Render() {
		init_transform();

		GLfloat angle = touch_angle + tilt_angle;

		s_mat *= scale_image(7.0, 7.0);

		t_mat *= move_image(0.0, -0.5);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.5);

		t_mat *= move_image(position, 0.2);
		draw_image(tex[head_state]);
	}

	Head() {
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 1500, 1500, 1);
	}

	~Head() {
		for(int i = 0; i < 3; ++i)
			glDeleteTextures(1, &tex[i]);
	}
};



class Body {
private:
	std::array<unsigned int, 1> tex{};
	std::array<const char*, 1> directory = {
		"res//alcy//body.png"
	};

	GLfloat position{};
	int head_state{};

public:
	void tell_head_state(int state) {
		head_state = state;
	}

	void tell_head_position(GLfloat pos) {
		position = pos;
	}

	void Render() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(0.0, -0.55);
		draw_image(tex[0]);
	}

	Body() {
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 1500, 1500, 1);
	}

	~Body() {
		glDeleteTextures(1, &tex[0]);
	}
};



class Hair {
private:
	std::array<unsigned int, 1> tex{};
	std::array<const char*, 1> directory = {
		"res//alcy//hair.png"
	};

	GLfloat tilt_angle{};

	GLfloat touch_angle{};

public:
	void tell_head_angle(GLfloat a) {
		tilt_angle = a;
	}

	void tell_touch_angle(GLfloat a) {
		touch_angle = a;
	}

	void Render() {
		init_transform();

		GLfloat angle = touch_angle + tilt_angle;

		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(0.0, -0.55 - angle / 200);
		draw_image(tex[0]);
	}

	Hair() {
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 1500, 1500, 1);
	}

	~Hair() {
		glDeleteTextures(1, &tex[0]);
	}
};


class Tail {
private:
	std::array<unsigned int, 1> tex{};
	std::array<const char*, 1> directory = {
		"res//alcy//tail.png"
	};

	GLfloat tilt_angle{};

	GLfloat touch_angle{};

public:
	void tell_head_angle(GLfloat a) {
		tilt_angle = a;
	}

	void tell_touch_angle(GLfloat a) {
		touch_angle = a;
	}


	void Render() {
		init_transform();
		
		GLfloat angle = tilt_angle + touch_angle;

		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(-0.19, -0.55);
		t_mat *= rotate_image(angle / 2);
		draw_image(tex[0]);
	}

	Tail() {
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 512, 512, 1);
	}

	~Tail() {
		glDeleteTextures(1, &tex[0]);
	}
};