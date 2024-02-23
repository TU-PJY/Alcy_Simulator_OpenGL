#include "gl_func.h"
#include "globalVar.h"
#include "screen.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "Icon.h"
#include "White.h"

void convertToGLCoord(int x, int y) {  //GL��ǥ��� ��ȯ
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void setDir() {  // ��Ű �ٶ󺸴� ������ �����Ѵ�
		if (mx / cam.zoom * ratio_ < -0.5 * ratio_)
			alcy.dir = l;
		else if (mx / cam.zoom * ratio_ > 0.5 * ratio_)
			alcy.dir = r;
		else
			alcy.dir = m;
}

void updateCursor() {  // ��Ű �Ӹ��� Ŀ���� �����ٴ�� Ŀ���� �ٲ��
	if ((mx / cam.zoom * ratio_ >= -0.4 && mx / cam.zoom * ratio_ <= 0.4) && (my / cam.zoom >= 0.1 && my / cam.zoom <= 0.5)) // ���ٵ��
		ui.handEnable = true;
	else
		ui.handEnable = false;

	if ((mx / cam.zoom * ratio_ >= -0.03 && mx / cam.zoom * ratio_ <= 0.03) && (my / cam.zoom >= -0.2 && my / cam.zoom <= -0.15))  // �� ������
		ui.fingerEnable = true;
	else
		ui.fingerEnable = false;
}

void selectIcon() {  // Ŀ���� ������ ����
	for (int i = 0; i < ICON_PART; i++) {
		GLfloat iconX = (-0.53 + i * 0.35);
		GLfloat iconY = -0.33;
		if ((iconX - 0.14 <= mx * ratio_ && mx * ratio_ <= iconX + 0.14) &&
			(iconY - 0.13 <= my && my <= iconY + 0.13)) {
			icon[i].isOnCursor = true;  // ���õ� �������� ��¦ ����������.
		}
		else {
			icon[i].isOnCursor = false;  // �� ���� �������� �״�� �ִ´�.
		}
	}
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
		ui.handNum = 0;
		alcy.headRot = 0;
		alcy.tailNum = 0;
		alcy.tailRot = 0;
		alcy.bodyRot = 0;
		alcy.touchEnable = false;
		channelTouch->stop();  // ���ٵ�⸦ �ߴ��� ��� �Ҹ��� �����Ѵ�.
	}
}

void clickSqueak() {
	if (ui.fingerEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak && !playFunc) {
		random_device rd;  mt19937 gen(rd());
		uniform_int_distribution <int> dis(1, 3);
		int randomSound = dis(gen);

		channelSqueak->stop();

		switch (randomSound) {  // 3���� �� �ϳ��� ����Ѵ�.
		case 1:
			ssystem->playSound(squeak1, 0, false, &channelSqueak);
			break;
		case 2:
			ssystem->playSound(squeak2, 0, false, &channelSqueak);
			break;
		case 3:
			ssystem->playSound(squeak3, 0, false, &channelSqueak);
			break;
		}

		alcy.squeak = true;
		lButtonDown = true;
	}
}

void executeFunc(int idx) {
	channelMenu->stop();
	ssystem->playSound(menuClick, 0, false, &channelMenu);
	functionOperationTime = 0;
	alcy.sleeping = false;  // ��� ���� �߿��� ���� ���� Ȯ���� ���� �ʴ´�.
	alcy.tired = false;
	alcy.measureTime = false;
	alcy.isLeave = false;
	alcy.confirmLeave = false;

	switch (idx) {  // ��� ���� ���� ���� �������� ������ ����
	case 0:
		if (!playFunc) {
			channelTheme->stop();
			channelMusic->stop();
			ssystem->playSound(music1, 0, false, &channelMusic);
			playFunc = true;
			beatDelay = 4.688;
			whiteTransparent = 1.0;
			icon[idx].operating = true;
			break;
		}
		else {
			cam.zoom = 1.0;  // �ٽ� ���� �ʱ�ȭ �Ѵ�.
			beatDelay = 0;
			alcy.beatX = 0;
			alcy.beatY = 0;
			whiteTransparent = 1.0;
			channelMusic->stop();
			ssystem->playSound(mainTheme, 0, false, &channelTheme);
			playFunc = false;
		}
		break;

	case 1:
		channelMusic->stop();
		ssystem->playSound(music2, 0, false, &channelMusic);
		break;

	case 2:
		channelMusic->stop();
		ssystem->playSound(music3, 0, false, &channelMusic);
		break;

	case 3:
		channelMusic->stop();
		ssystem->playSound(music4, 0, false, &channelMusic);
		break;
	}

	musicTrack = idx;  // ������ �����ܿ� ���� �̹����� �޶�����.
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

				cam.zoom = 1.0;  // ����� �������� �ϹǷ� ���� �ʱ�ȭ�Ѵ�.
				alcy.dir = m;  // ��Ű �Ӹ��� �ʱ�ȭ ��Ų��.
				alcy.headPos = 0;

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
		if (gameStarted) {
			cam.camX = (0.0 - mx) / 10 / cam.zoom;
			cam.camY = (0.0 - my) / 10 / cam.zoom;

			if (!ui.menuEnable && !playFunc) 
				updateCursor();
			if(ui.menuEnable && ui.menuOpened)
				selectIcon();

			if(!playFunc)
				setDir();
			

			alcy.isLeave = false;  // �����̸� ��Ʈ���� �����Ѵ�.
		}

	glutPostRedisplay();
}

void Motion(int x, int y) {  // Ŭ�� �� ���� ���
	if (gameStarted) {
		if (!alcy.touchEnable) {  // ��Ű�� ���ٵ�� ���߿��� ���콺 ������Ʈ�� ���� �ʴ´�.
			convertToGLCoord(x, y);
			cam.camX = (0.0 - mx) / 10 / cam.zoom;
			cam.camY = (0.0 - my) / 10 / cam.zoom;

			if (!ui.menuEnable && !playFunc)
				updateCursor();
			if (ui.menuEnable && ui.menuOpened)
				selectIcon();

			if (!playFunc)
				setDir();
		}

		alcy.isLeave = false;  // �����̸� ��Ʈ���� �����Ѵ�.
	}

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // ���콺 ��
	if (gameStarted && !ui.menuEnable && !playFunc) {
		if (dir > 0) {
			channelScroll->stop();
			ssystem->playSound(scroll, 0, false, &channelScroll);

			cam.zoomAcc = 0.25;
			cam.zoomEnable = true;

			alcy.isLeave = false;
		}
		else if (dir < 0) {
			channelScroll->stop();
			ssystem->playSound(scroll, 0, false, &channelScroll);

			if (cam.zoom > 1.0) {
				cam.zoomAcc = -0.25;
				cam.zoomEnable = true;
			}

			alcy.isLeave = false;
		}
	}
	
	return;
}