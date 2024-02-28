#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include "config.h"
// ���⿡ ������Ʈ ���� ���� ����

// ���콺
extern GLfloat mx, my;  // ���콺 ��ǥ
extern bool lButtonDown;  // ���콺 Ŭ�� ����
extern bool mouseClickEnable;  // ���콺 Ŭ�� ���� ����
extern bool startIntro;  // ī�޶� ��Ʈ�� �ִϸ��̼� Ȱ��ȭ ����
extern bool gameStarted;  // ���� ������ �˸��� ����

// ������
extern int lastElapsedTime, elapsedTime;
extern GLfloat fs;  // frame sync, ������ ����ȭ

// �޴��� ���
extern bool playFunc; // ��� ���� ����
extern int funcNumber;  // ��� ��ȣ
extern double beatDelay;  // ���� ȿ���� ���Ǵ� ������ ����
extern GLfloat interpolation;  // ���� ȿ�� ������ �����ϴ� ���� 
extern GLfloat interval, playTime;  // ���� ���� ����, ���� ��� ���� �ð�
extern GLfloat beatAcc;  // ���� ȿ�� ���Ӱ�
extern GLfloat beatVal;  // ���� ȿ�� ��ġ
extern GLfloat functionOperationTime;  // ��� ���� �ð�

// ���� / ����â
extern int ExitOrInfo;  // ���� �Ǵ� ����â ���� ����, 0: ����, 1: ����â
extern bool setInfo;  // true�� �� ���� ǥ�� Ȱ��ȭ

// UI
extern GLfloat whiteTransparent;  // �� ��� ����

// �ε�
extern bool LoadingEnd;   // ���ҽ� �ε� �Ϸ� ����

#endif