#include "../header/gl_func.h"
#include "../header/Camera.h"
#include "../header/mode_header.h"

GLfloat mx, my;

// ������ ��ǥ�� -> OpenGL ��ǥ�� ��ȯ
void convert_cursor_position(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}
