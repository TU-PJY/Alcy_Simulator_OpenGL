#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include "config.h"
// ���⿡ ������Ʈ ���� ���� ����

// ���콺
extern GLfloat mx, my;  // ���콺 ��ǥ
extern bool lButtonDown;  // ���콺 Ŭ�� ����
extern bool mouseClickEnable;  // ���콺 Ŭ�� ���� ����
extern bool startIntro;
extern bool gameStarted;

// ������
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, ������ ����ȭ

// ���� ���, true�� �� �ش��ϴ� ������ ����ǰ�, ���ڿ� ���߾� ��Ű�� �����̰� ī�޶� �����δ�.
extern bool playFunc;  
extern int musicTrack;
extern double beatDelay;  // ���� ȿ���� ���Ǵ� ������ ����
extern GLfloat interpolation;  // ���� ȿ�� �ӵ��� �����ϴ� ���� 
extern GLfloat interval, playTime;  // ���� ����, ���� �÷��� �ð�
extern GLfloat beatAcc;
extern GLfloat beatVal;
extern GLfloat functionOperationTime;

extern GLfloat whiteTransparent;  // �� ��� ����

#endif