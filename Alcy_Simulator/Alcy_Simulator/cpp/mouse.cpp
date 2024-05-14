#include "../header/gl_func.h"
#include "../header/Camera.h"
#include "../header/mode_header.h"

GLfloat mx, my;

// 윈도우 좌표계 -> OpenGL 좌표계 변환
void convert_cursor_position(int x, int y) {  //GL좌표계로 변환
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}
