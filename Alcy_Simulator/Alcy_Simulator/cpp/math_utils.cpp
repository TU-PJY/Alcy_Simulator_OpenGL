#include "../header/math_util.h"

// 두 오브젝트 간의 거리 계산
GLfloat calc_distance(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
