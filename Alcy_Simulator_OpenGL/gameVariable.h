#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H
#include "config.h"
// ���⿡ ���� ���� ����

// ���콺
extern GLfloat mx, my;

extern GLfloat ratio;

// ������
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, ������ ����ȭ

// Ŀ��
extern bool cursorEnable;  // Ŀ�� ���� ����
extern bool handEnable;  // ��Ŀ�� ��ȯ ����
extern GLfloat handX;  // ��Ŀ�� x��ǥ, ���ٵ�� �� ���
extern GLfloat handNum;  // �� Ŀ�� �ִϸ��̼ǿ� ���Ǵ� ��ġ

// ������ ������
extern bool exitEnable;
extern GLfloat exitTransparent; // ������ ������ ����

//��
extern bool tipEnable;
extern GLfloat tipTransparent;

#endif