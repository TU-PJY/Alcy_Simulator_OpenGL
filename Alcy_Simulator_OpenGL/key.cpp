// 키보드 조작
#include "gl_func.h"
#include "globalVar.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "Icon.h"

void openMenu() {
	channelMenu->stop();
	ssystem->playSound(menuOpen, 0, false, &channelMenu);
	ui.menuEnable = true;
	ui.handEnable = false;
	ui.fingerEnable = false;
	ui.exitEnable = false;
	ui.infoEnable = false;
	ui.menuTransparent = 1;
	ui.menuSizeX = 0;
	ui.menuSizeY = 0;
	ui.menuAcc = 0.2;

	if (cam.camL || cam.camR) {
		cam.camL = false;
		cam.camR = false;
	}

	if (!cam.camL && !cam.camR && alcy.touchEnable) {
		ui.handX = 0;
		ui.handNum = 0;
		alcy.headRot = 0;
		alcy.tailNum = 0;
		alcy.tailRot = 0;
		alcy.bodyRot = 0;
		alcy.touchEnable = false;
		channelTouch->stop();
	}
}

void closeMenu() {
	channelMenu->stop();
	ssystem->playSound(menuClose, 0, false, &channelMenu);

	ui.menuEnable = false;
	ui.menuOpened = false;  // 더 이상 커서와 아이콘이 상호작용하지 않는다.
	ui.menuTransparent = 0.743;
	ui.menuSizeX = 1.02;
	ui.menuSizeY = 0.51;
	ui.menuAcc = 0.2;

	for (int i = 0; i < ICON_PART; i++)
		icon[i].isOnCursor = false;
}

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		if (!escSoundPlayed && !ui.menuEnable) {  // 중복 재생 방지
			channelEscDown->stop();
			ssystem->playSound(escDown, 0, false, &channelEscDown);
			escSoundPlayed = true;
		}
		if (!ui.menuEnable) {
			ui.exitEnable = true;
			alcy.isLeave = false;
		}
		break;

	case 32:  // space
		if (!gameStarted && INTRO == 1) {  // 게임 시작 시 인트로 출력
			startIntro = true;  // 카메라 애니메이션 활성화
			ui.intro = true;
		}

		else if (gameStarted) {  // 게임 시작 이후에는 메뉴를 여는 기능을 한다.
			if (!ui.menuEnable)
				openMenu();
			else
				closeMenu();

			alcy.isLeave = false;
		}
		break;

	case 9:  // tab
		if (gameStarted) {
			channelClick->stop();
			ssystem->playSound(click, 0, false, &channelClick);

			if (!ui.tipEnable) {
				ui.tipEnable = true;
				break;
			}
			else
				ui.tipEnable = false;
			alcy.isLeave = false;
		}
		break;

	case 'q':  // 카메라 좌측 회전
		if (gameStarted) {
			if (!alcy.touchEnable && !ui.menuEnable && !playMusic) {
				cam.camL = true;
				mouseClickEnable = false;
			}
			alcy.isLeave = false;
		}
		break;

	case 'e':  // 카메라 우측 회전
		if (gameStarted) {
			if (!alcy.touchEnable && !ui.menuEnable && !playMusic) {
				cam.camR = true;
				mouseClickEnable = false;
			}
			alcy.isLeave = false;
		}
		break;

	case 'i':  // info 보기
		if (gameStarted && !ui.menuEnable) {
			if (!escSoundPlayed) {  // 중복 재생 방지
				channelEscDown->stop();
				ssystem->playSound(escDown, 0, false, &channelEscDown);
				escSoundPlayed = true;
			}
			ui.infoEnable = true;  // info 활성화
		}
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void keyUp(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		if (!ui.menuEnable) {
			channelEscUp->stop();
			ssystem->playSound(escUp, 0, false, &channelEscUp);
			escSoundPlayed = false;
		}

		ui.exitEnable = false;
		break;

	case 'q':  // 카메라 우측 회전 해제
		cam.camL = false;
		alcy.tiltSoundPlayed = false;
		break;

	case 'e':  // 카메라 좌측 회전 해제
		cam.camR = false;
		alcy.tiltSoundPlayed = false;
		break;

	case 'i':
		if (gameStarted && !ui.menuEnable) {
			channelEscUp->stop();
			ssystem->playSound(escUp, 0, false, &channelEscUp);
			escSoundPlayed = false;

			ui.infoEnable = false;
		}
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
