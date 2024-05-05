#include "../header/gl_func.h"
#include "../header/Camera.h"

GLfloat mx, my;

// ������ ��ǥ�� -> OpenGL ��ǥ�� ��ȯ
void convert_cursor_position(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}


void mouse_button(int button, int state, int x, int y) {  // ���콺 Ŭ��
	// Ȩ ��� ���콺 ����
	if (fw.get_current_mode() == "home_mode") {
		auto ptr = fw.get_ptr(cursor_layer, 0);

		if (ptr != nullptr) {
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
				ptr->mouse_left_button_down(button, state);

			else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
				ptr->mouse_left_button_up(button, state);
		}
	}

	glutPostRedisplay();
}


// Ŭ�� ���� ���� ���
void mouse_passive_motion(int x, int y) { 
	convert_cursor_position(x, y);

	glutPostRedisplay();
}


// Ŭ�� �� ���� ���
void mouse_motion(int x, int y) {  
	convert_cursor_position(x, y);

	glutPostRedisplay();
}


// ���콺 ��
void mouse_wheel(int button, int dir, int x, int y) { 

	if (fw.get_current_mode() == "home_mode")
		cam.scroll(dir);
}

