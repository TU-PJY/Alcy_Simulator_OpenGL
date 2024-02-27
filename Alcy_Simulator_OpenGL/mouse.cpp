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

void convertToGLCoord(int x, int y) {  //GL좌표계로 변환
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
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
		alcy.headRot = 0;
		alcy.tailNum = 0;
		alcy.tailRot = 0;
		alcy.bodyRot = 0;
		alcy.touchEnable = false;

		ui.handNum = 0;

		channelTouch->stop();  // 쓰다듬기를 중단할 경우 소리를 정지한다.
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

	alcy.dir = m;  // 알키 데이터 초기화
	alcy.headPos = 0;
	alcy.headRot = 0;
	alcy.headNum = 0;
	alcy.tailRot = 0;
	alcy.tailNum = 0;

	alcy.sleeping = false;  // 알키의 모든 상태 해제
	alcy.tired = false;
	alcy.measureTime = false;
	alcy.isLeave = false;
	alcy.confirmLeave = false;
	alcy.squeak = false;
	alcy.squeakTime = 0;
}

void initFunc(int idx) {  // 메뉴바 기능 실행 또는 중지 시 초기화 하는 함수
	cam.zoom = 1.0;  // 카메라 초기화
	cam.camRot = 0;

	initAlcyMouse();

	beatVal = 0;
	whiteTransparent = 1.0;

	for (int i = 0; i < ICON_PART; i++) {  // 아이콘 효과 초기화
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
		for (int i = 0; i < ICON_PART; i++)  // 기능 실행 도중 다른 아이콘을 선택할 경우 먼저 모든 기능들을 false 상태로 만든다.
			icon[i].operating = false;

		playFunc = false;
	}

	switch (idx) {  // 기능 실행 도중 같은 아이콘을 누르면 중지
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

	funcNumber = idx;  // 선택한 아이콘에 따라 이미지가 달라진다.
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

				icon[0].playClickSound();
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

		if (gameStarted) 
			alcy.isLeave = false;  // 움직이면 컨트롤을 감지한다.
		

	glutPostRedisplay();
}

void Motion(int x, int y) {  // 클릭 할 때의 모션
	if(!alcy.touchEnable)
		convertToGLCoord(x, y);

	if (gameStarted)
		alcy.isLeave = false;  // 움직이면 컨트롤을 감지한다.

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // 마우스 휠
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