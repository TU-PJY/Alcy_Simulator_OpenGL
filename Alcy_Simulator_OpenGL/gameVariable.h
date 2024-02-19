#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H
#include "config.h"
// ���⿡ ���� ���� ����

// ���콺
extern GLfloat mx, my;

// ī�޶�
extern GLfloat ratio;  // ȭ�� ��Ⱦ��;
extern GLfloat camX, camY;  // ���� ȭ�� ī�޶�
extern GLfloat camRot;  // ī�޶� ȸ�� ����
extern GLfloat zoom;  // ī�޶� ��
extern GLfloat zoomAcc;  // �� ���ӵ�
extern bool zoomEnable;  // �� ����
extern bool camR, camL;  // ī�޶� �¿� ȸ��

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

//// ��Ű ���� ���� ////
extern int dir;  // ��Ű �ٶ󺸴� ����, �ʱⰪ m
extern bool blinkEnable; // �� ������ ����, true�� �� ������ Ȱ��ȭ
extern time_t startTime, endTime, blinkTime;  // �� ������ ���� Ÿ�̸�
extern GLfloat blinkInterval;  // �� ������ ����, �⺻ 3��
extern GLfloat keepTimer;  // ���� ���� ���¸� ���� ª�� �����Ѵ�
extern bool touchEnable;  // ��Ű �Ӹ� ���ٵ�� ����

//��Ű �Ӹ�, �� ���� ������
extern GLfloat headPos;  // ���� ��ȯ �� ��ȯ�� �������� ������
extern GLfloat headRot;  // ���ٵ�� �� �Ӹ� ȸ�� ����
extern GLfloat tailRot;  // ���ٵ�� �� ���� ȸ�� ����
extern GLfloat bodyRot;  // ���ٵ�� �� ���� ȸ�� ����
extern GLfloat tailNum; // ���� ȸ���� ���Ǵ� ��ġ
extern bool headTiltR, headTiltL;  // ī�޶� ��￴�� �� ��Ű �Ӹ� ����̱� ����

#endif