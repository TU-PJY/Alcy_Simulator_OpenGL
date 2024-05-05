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
	GLuint VAO{};
	std::array<unsigned int, 3> tex_eye{};
	std::array<unsigned int, 3> tex_blink{};
	std::array<unsigned int, 3> tex_dot{};
	std::array<unsigned int, 3> tex_brow{};

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

	// 눈 깜빡임 상태, true일 시 blink 출력, false일 시 eye 출력
	bool blink_state{};

	GLfloat position{};
	GLfloat tilt_angle{};

	GLfloat touch_angle{};

	// 머리 방향
	int head_state{};
	
	// 머리 회전 여부
	bool head_rotate_state{};


public:
	void tell_head_state(int state) {
		head_state = state;
	}

	// 눈 깜빡임 상태 알리기
	void tell_blink_state(bool state) {
		blink_state = state;
	}

	// 머리 위치 알리기
	void tell_head_position(GLfloat pos) {
		position = pos;
	}

	void tell_head_angle(GLfloat a) {
		tilt_angle = a;
	}

	void tell_touch_angle(GLfloat a) {
		touch_angle = a;
	}

	// 머리 회전 여부 활성화 / 비활성화
	void enable_state_static() {
		head_rotate_state = true;
	}
	void unable_state_static() {
		head_rotate_state = false;
	}


	void render_eye() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.0);

		
		GLfloat x = mx / 50;
		GLfloat y = my / 50;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.5);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.5);

		if(!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_eye[head_state], VAO);
	}


	void render_blink() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.0);

		GLfloat x = mx / 50;
		GLfloat y = my / 50;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.5);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.5);

		if (!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_blink[head_state], VAO);
	}


	void render_dot() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.0);

		GLfloat x = mx / 25;
		GLfloat y = my / 25;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.5);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.5);
		
		if (!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_dot[head_state], VAO);
	}
	

	void render_brow() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(position, 0.0);

		GLfloat x = mx / 50;
		GLfloat y = my / 50;

		GLfloat angle = touch_angle + tilt_angle;

		t_mat *= move_image(0.0, -0.5);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.5);

		if (!head_rotate_state)
			t_mat *= move_image(x, y);
		draw_image(tex_brow[head_state], VAO);
	}


	void render() {
		// blink state가 true이면 눈을 깜빡이는 것
		if (!blink_state) {
			render_eye();
			render_dot();
		}

		else
			render_blink();

		render_brow();
	}

	Face() {
		set_canvas(VAO);

		for (int i = 0; i < directory_eye.size(); ++i) 
			set_texture(tex_eye[i], directory_eye[i], 1500, 1500, 1);

		for (int i = 0; i < directory_blink.size(); ++i)
			set_texture(tex_blink[i], directory_blink[i], 1500, 1500, 1);

		for (int i = 0; i < directory_dot.size(); ++i)
			set_texture(tex_dot[i], directory_dot[i], 1500, 1500, 1);

		for (int i = 0; i < directory_brow.size(); ++i)
			set_texture(tex_brow[i], directory_brow[i], 1500, 1500, 1);
	}
};



class Head {
private:
	GLuint VAO{};
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


	void render() {
		init_transform();

		GLfloat angle = touch_angle + tilt_angle;

		s_mat *= scale_image(7.0, 7.0);

		t_mat *= move_image(0.0, -0.5);
		t_mat *= rotate_image(angle);
		t_mat *= move_image(0.0, 0.5);

		t_mat *= move_image(position, 0.0);
		draw_image(tex[head_state], VAO);
	}

	Head() {
		set_canvas(VAO);
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 1500, 1500, 1);
	}
};



class Body {
private:
	GLuint VAO{};
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

	void render() {
		init_transform();
		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(0.0, -0.75);
		draw_image(tex[0], VAO);
	}

	Body() {
		set_canvas(VAO);
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 1500, 1500, 1);
	}
};



class Hair {
private:
	GLuint VAO{};
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

	void render() {
		init_transform();

		GLfloat angle = touch_angle + tilt_angle;

		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(0.0, -0.75 - angle / 200);
		draw_image(tex[0], VAO);
	}

	Hair() {
		set_canvas(VAO);
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 1500, 1500, 1);
	}
};


class Tail {
private:
	GLuint VAO{};
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


	void render() {
		init_transform();
		
		GLfloat angle = tilt_angle + touch_angle;

		s_mat *= scale_image(7.0, 7.0);
		t_mat *= move_image(-0.19, -0.75);
		t_mat *= rotate_image(angle / 2);
		draw_image(tex[0], VAO);
	}

	Tail() {
		set_canvas(VAO);
		for (int i = 0; i < directory.size(); ++i)
			set_texture(tex[i], directory[i], 1500, 1500, 1);
	}
};