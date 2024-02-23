#include "gl_func.h"
#include "globalVar.h"
#include "screen.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "Icon.h"
#include "White.h"

void convertToGLCoord(int x, int y) {  //GL좌표계로 변환
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void setDir() {  // 알키 바라보는 방향을 결정한다
		if (mx / cam.zoom * ratio_ < -0.5 * ratio_)
			alcy.dir = l;
		else if (mx / cam.zoom * ratio_ > 0.5 * ratio_)
			alcy.dir = r;
		else
			alcy.dir = m;
}

void updateCursor() {  // 알키 머리에 커서를 가져다대면 커서가 바뀐다
	if ((mx / cam.zoom * ratio_ >= -0.4 && mx / cam.zoom * ratio_ <= 0.4) && (my / cam.zoom >= 0.1 && my / cam.zoom <= 0.5)) // 쓰다듬기
		ui.handEnable = true;
	else
		ui.handEnable = false;

	if ((mx / cam.zoom * ratio_ >= -0.03 && mx / cam.zoom * ratio_ <= 0.03) && (my / cam.zoom >= -0.2 && my / cam.zoom <= -0.15))  // 코 누르기
		ui.fingerEnable = true;
	else
		ui.fingerEnable = false;
}

void selectIcon() {  // 커서로 아이콘 선택
	for (int i = 0; i < ICON_PART; i++) {
		GLfloat iconX = (-0.53 + i * 0.35);
		GLfloat iconY = -0.33;
		if ((iconX - 0.14 <= mx * ratio_ && mx * ratio_ <= iconX + 0.14) &&
			(iconY - 0.13 <= my && my <= iconY + 0.13)) {
			icon[i].isOnCursor = true;  // 선택된 아이콘은 살짝 투명해진다.
		}
		else {
			icon[i].isOnCursor = false;  // 그 외의 아이콘은 그대로 있는다.
		}
	}
}

void startTouch() {
	if (ui.handEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak && !playFunc) {
		channelTouch->stop();
		ssystem->playSound(touch, 0, false, &channelTouch);   // 쓰다듬기 시작한 순간부터 소리를 반복 재생한다.
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
		channelTouch->stop();  // 쓰다듬기를 중단할 경우 소리를 정지한다.
	}
}

void clickSqueak() {
	if (ui.fingerEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak && !playFunc) {
		random_device rd;  mt19937 gen(rd());
		uniform_int_distribution <int> dis(1, 3);
		int randomSound = dis(gen);

		channelSqueak->stop();

		switch (randomSound) {  // 3가지 중 하나를 재생한다.
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
	alcy.sleeping = false;  // 기능 실행 중에는 조작 유무 확인을 하지 않는다.
	alcy.tired = false;
	alcy.measureTime = false;
	alcy.isLeave = false;
	alcy.confirmLeave = false;

	switch (idx) {  // 기능 실행 도중 같은 아이콘을 누르면 중지
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
			cam.zoom = 1.0;  // 다시 줌을 초기화 한다.
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

	musicTrack = idx;  // 선택한 아이콘에 따라 이미지가 달라진다.
}

void clickMenuIcon() {
	if (ui.menuEnable) {  // 아이콘 선택 시 실행되면서 메뉴바가 닫힌다.
		for (int i = 0; i < ICON_PART; i++) {
			if (ui.menuOpened && icon[i].isOnCursor) {
				ui.menuEnable = false;
				ui.menuOpened = false;  // 더 이상 커서와 아이콘이 상호작용하지 않는다.
				ui.menuTransparent = 0.743;  // 메뉴가 닫힌다.
				ui.menuSizeX = 1.02;
				ui.menuSizeY = 0.51;
				ui.menuAcc = 0.2;

				cam.zoom = 1.0;  // 기능을 실행해햐 하므로 줌을 초기화한다.
				alcy.dir = m;  // 알키 머리를 초기화 시킨다.
				alcy.headPos = 0;

				executeFunc(i);

				for (int i = 0; i < ICON_PART; i++)
					icon[i].isOnCursor = false;
			}
		}
	}
}

void Mouse(int button, int state, int x, int y) {  // 마우스 클릭
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

void pMotion(int x, int y) {  // 클릭 안할 때의 모션
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
			

			alcy.isLeave = false;  // 움직이면 컨트롤을 감지한다.
		}

	glutPostRedisplay();
}

void Motion(int x, int y) {  // 클릭 할 때의 모션
	if (gameStarted) {
		if (!alcy.touchEnable) {  // 알키를 쓰다듬는 도중에는 마우스 업데이트를 하지 않는다.
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

		alcy.isLeave = false;  // 움직이면 컨트롤을 감지한다.
	}

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // 마우스 휠
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