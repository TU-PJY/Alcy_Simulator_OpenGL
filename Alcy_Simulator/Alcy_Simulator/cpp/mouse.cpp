#include "../header/gl_func.h"

GLfloat mx, my;

// 윈도우 좌표계 -> OpenGL 좌표계 변환
void convert_cursor_position(int x, int y) {  //GL좌표계로 변환
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}


void mouse_button(int button, int state, int x, int y) {  // 마우스 클릭
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

	}

	glutPostRedisplay();
}


// 클릭 안할 때의 모션
void mouse_passive_motion(int x, int y) { 
	convert_cursor_position(x, y);

	glutPostRedisplay();
}


// 클릭 할 때의 모션
void mouse_motion(int x, int y) {  
	convert_cursor_position(x, y);

	glutPostRedisplay();
}


// 마우스 휠
void mouse_wheel(int button, int dir, int x, int y) { 
	if (dir > 0) {

	}

	else if (dir < 0) {

	}
}

