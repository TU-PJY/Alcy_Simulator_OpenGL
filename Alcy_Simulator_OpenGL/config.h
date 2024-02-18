// 전역 변수, 변수 열거형, 모델 출력 개수는 이곳에 선언 
// 프로젝트의 중요 헤더 파일들은 이 파일이 담당 
// 나머지 헤더 파일들은 이 파일에 대해 의존성을 가지고 있기 때문에 이 파일을 절대 삭제해서는 안됨.
#ifndef CONFIG_H
#define CONFIG_H
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4305)
#pragma warning(disable: 4326)
#pragma warning(disable: 4244)
#include <windows.h>
#include <gl/glm/glm.hpp> //  이 include들을 다른데로 옮기지 말 것!
#include <gl/glm/ext.hpp> 
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#define IMAGE_COUNT 20

using namespace std;

enum dir {  // 알키 바라보는 방향
	l, m, r
};

#endif
