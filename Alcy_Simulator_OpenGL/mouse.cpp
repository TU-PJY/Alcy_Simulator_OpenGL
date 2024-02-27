#include "gl_func.h"
#include "globalVar.h"
#include "screen.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "Icon.h"
#include "White.h"
#include "Note.h"

void convertToGLCoord(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void startTouch() {
	if (ui.handEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak && !playFunc) {
		channelTouch->stop();
		ssystem->playSound(touch, 0, false, &channelTouch);   // ���ٵ�� ������ �������� �Ҹ��� �ݺ� ����Ѵ�.

		alcy.touchEnable = true;
		lButtonDown = true;
	}
}

void stopTouch() {
	if (ui.handEnable && mouseClickEnable && alcy.touchEnable) {
		alcy.headRot = 0;
		alcy.tailNum = 0;
		alcy.tailRot = 0;
		alcy.bodyRot = 0;
		alcy.touchEnable = false;

		ui.handNum = 0;

		channelTouch->stop();  // ���ٵ�⸦ �ߴ��� ��� �Ҹ��� �����Ѵ�.
	}
}

void clickSqueak() {
	if (ui.fingerEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak && !playFunc) {
		alcy.playAlcySqueakSound();
		lButtonDown = true;
	}
}

void initAlcyMouse() {
	alcy.beatX = 0;
	alcy.beatY = 0;

	alcy.dir = m;  // ��Ű ������ �ʱ�ȭ
	alcy.headPos = 0;
	alcy.headRot = 0;
	alcy.headNum = 0;
	alcy.tailRot = 0;
	alcy.tailNum = 0;

	alcy.sleeping = false;  // ��Ű�� ��� ���� ����
	alcy.tired = false;
	alcy.measureTime = false;
	alcy.isLeave = false;
	alcy.confirmLeave = false;
	alcy.squeak = false;
	alcy.squeakTime = 0;
}

void initFunc(int idx) {  // �޴��� ��� ���� �Ǵ� ���� �� �ʱ�ȭ �ϴ� �Լ�
	cam.zoom = 1.0;  // ī�޶� �ʱ�ȭ
	cam.camRot = 0;

	initAlcyMouse();

	beatVal = 0;
	whiteTransparent = 1.0;

	for (int i = 0; i < ICON_PART; i++) {  // ������ ȿ�� �ʱ�ȭ
		icon[i].iconBeatRot = 0;
		icon[i].iconBeatX = 0;
	}
	
	if (!icon[idx].operating) {
		icon[idx].operating = true;
		playFunc = true;
	}

	else {
		icon[idx].operating = false;
		playFunc = false;
	}

	functionOperationTime = 0;
}

void executeFunc(int idx) {
	if (idx != funcNumber && playFunc) {
		for (int i = 0; i < ICON_PART; i++)  // ��� ���� ���� �ٸ� �������� ������ ��� ���� ��� ��ɵ��� false ���·� �����.
			icon[i].operating = false;

		playFunc = false;
	}

	switch (idx) {  // ��� ���� ���� ���� �������� ������ ����
	case 0:
		if (!playFunc) {
			channelTheme->stop();
			channelMusic->stop();
			ssystem->playSound(music1, 0, false, &channelMusic);
			initFunc(idx);
			beatDelay = 4.688;  // 128 bpm
			break;
		}
		else {
			channelMusic->stop();
			ssystem->playSound(mainTheme, 0, false, &channelTheme);
			initFunc(idx);
		}
		break;

	case 1:
		if (!playFunc) {
			channelTheme->stop();
			channelMusic->stop();
			ssystem->playSound(music2, 0, false, &channelMusic);
			initFunc(idx);
			beatDelay = 5.455;  // 110 bpm
			break;
		}
		else {
			channelMusic->stop();
			ssystem->playSound(mainTheme, 0, false, &channelTheme);
			initFunc(idx);
		}
		break;

	case 2:
		if (!playFunc) {
			channelTheme->stop();
			channelMusic->stop();
			ssystem->playSound(music3, 0, false, &channelMusic);
			initFunc(idx);

			note.num = 0; note.num2 = 0;
			note.rot = 0; note.noteTransparent = 0;
			note.x = 0.4; note.y = 0.4;

			break;
		}
		else {
			channelMusic->stop();
			ssystem->playSound(mainTheme, 0, false, &channelTheme);
			initFunc(idx);
		}
		break;

	case 3:
		if (!playFunc) {
			channelTheme->stop();
			channelMusic->stop();
			ssystem->playSound(music4, 0, false, &channelMusic);
			initFunc(idx);
			break;
		}
		else {
			channelMusic->stop();
			ssystem->playSound(mainTheme, 0, false, &channelTheme);
			initFunc(idx);
		}
		break;
	}

	funcNumber = idx;  // ������ �����ܿ� ���� �̹����� �޶�����.
}

void clickMenuIcon() {
	if (ui.menuEnable) {  // ������ ���� �� ����Ǹ鼭 �޴��ٰ� ������.
		for (int i = 0; i < ICON_PART; i++) {
			if (ui.menuOpened && icon[i].isOnCursor) {
				ui.menuEnable = false; 
				ui.menuOpened = false;  // �� �̻� Ŀ���� �������� ��ȣ�ۿ����� �ʴ´�.
				
				ui.menuTransparent = 0.743;  // �޴��� ������.
				ui.menuSizeX = 1.02; 
				ui.menuSizeY = 0.51;
				ui.menuAcc = 0.2;

				icon[0].playClickSound();
				executeFunc(i);

				for (int i = 0; i < ICON_PART; i++)
					icon[i].isOnCursor = false;
			}
		}
	}
}

void Mouse(int button, int state, int x, int y) {  // ���콺 Ŭ��
	if (gameStarted) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			startTouch();
			clickSqueak();
			clickMenuIcon();
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			stopTouch();
			lButtonDown = false;
		}
	}
	glutPostRedisplay();
}

void pMotion(int x, int y) {  // Ŭ�� ���� ���� ���
		convertToGLCoord(x, y);

		if (gameStarted) 
			alcy.isLeave = false;  // �����̸� ��Ʈ���� �����Ѵ�.
		

	glutPostRedisplay();
}

void Motion(int x, int y) {  // Ŭ�� �� ���� ���
	if(!alcy.touchEnable)
		convertToGLCoord(x, y);

	if (gameStarted)
		alcy.isLeave = false;  // �����̸� ��Ʈ���� �����Ѵ�.

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // ���콺 ��
	if (gameStarted && !ui.menuEnable && !playFunc) {
		if (dir > 0) {
			if (cam.zoom <= 2.615) {
				channelScroll->stop();
				ssystem->playSound(scroll, 0, false, &channelScroll);

				cam.zoomAcc = 0.25;
				cam.zoomEnable = true;
			}

			alcy.isLeave = false;
		}

		else if (dir < 0) {
			if (cam.zoom > 1.0) {
				channelScroll->stop();
				ssystem->playSound(scroll, 0, false, &channelScroll);

				cam.zoomAcc = -0.25;
				cam.zoomEnable = true;
			}

			alcy.isLeave = false;
		}
	}
	
	return;
}