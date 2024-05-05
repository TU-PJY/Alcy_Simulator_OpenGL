// 키보드 조작
#include "gl_func.h"
#include "globalVar.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "Icon.h"

void openMenu() {  // 메뉴바 열기
	channelMenu->stop();
	ssystem->playSound(menuOpen, 0, false, &channelMenu);

	ui.menuEnable = true;  // 메뉴바 활성화

	ui.handEnable = false;  // 커서를 기본 이미지로 초기화
	ui.fingerEnable = false;
	ui.exitEnable = false;  // 종료 중 또는 정보 창 띄우는 중이라면 비활성화
	ui.infoEnable = false;

	ui.menuTransparent = 1; // 메뉴바 값 초기화
	ui.menuSizeX = 0;
	ui.menuSizeY = 0;
	ui.menuAcc = 0.2;

	if (cam.camL || cam.camR) {  // 카메라 회전 중이라면 회전 비활성화
		cam.camL = false;
		cam.camR = false;
	}

	if (!cam.camL && !cam.camR && alcy.touchEnable) {  // 쓰다듬기 중이라면 쓰다듬기 중지 후 알키 및 커서 데이터 초기화
		ui.handX = 0;
		ui.handNum = 0;
		alcy.headRot = 0;
		alcy.tailNum = 0;
		alcy.tailRot = 0;
		alcy.bodyRot = 0;
		alcy.touchEnable = false;

		channelTouch->stop();  // 쓰다듬기 소리 중지
	}
}

void closeMenu() {  // 메뉴바 닫기
	channelMenu->stop();
	ssystem->playSound(menuClose, 0, false, &channelMenu);

	ui.menuEnable = false;
	ui.menuOpened = false;  // 더 이상 커서와 아이콘이 상호작용하지 않는다.
	ui.menuTransparent = 0.743;
	ui.menuSizeX = 1.02;
	ui.menuSizeY = 0.51;
	ui.menuAcc = 0.2;

	lButtonDown = false;

	for (int i = 0; i < ICON_PART; i++)  // 아이콘 커서 반응 비활성화
		icon[i].isOnCursor = false;
}

void initAlcyKey() {  // 알키 초기화
	if (alcy.touchEnable) { // 쓰다듬기 도중 특정 키를 누를경우 취소한다.
		ui.handNum = 0;
		alcy.headNum = 0;
		alcy.tailNum = 0;
		alcy.headRot = 0;
		alcy.tailRot = 0;
		alcy.bodyRot = 0;
	}

	ui.handEnable = false; 
	ui.fingerEnable = false;  
	alcy.touchEnable = false;

	lButtonDown = false;

	channelTouch->stop();  // 쓰다듬기를 중단할 경우 소리를 정지한다.
}

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		if (setInfo) {  // 정보창을 띄웠을 때에는 정보창을 끄는 역할을 한다.
			if (!escSoundPlayed) { // 1회만 사운드를 출력하도록 한다.
				channelEscDown->stop();
				ssystem->playSound(escDown, 0, false, &channelEscDown);
				escSoundPlayed = true;
			}
			setInfo = false;
			alcy.isLeave = false;
			break;
		}

		if (!ui.menuEnable && !ui.infoEnable) { // 메뉴바와 정보창이 비활성화 상태일 때에만 사용 가능
			if (!escSoundPlayed) {
				channelEscDown->stop();
				ssystem->playSound(escDown, 0, false, &channelEscDown);
				escSoundPlayed = true;
				ui.exitTransparent = 0.0;  // 나가기 아이콘 투명도 초기화
			}

			if (!setInfo) {  // 정보창을 띄우지 않았을 때만 종료 가능
				ExitOrInfo = 0;  // ui클래스에서 종료로 인식하게 된다.
				ui.exitEnable = true;  // 나가기 활성화
			}
		}
		alcy.isLeave = false;  // 컨트롤 감지
		break;

	case 32:  // space
		if (!gameStarted && INTRO == 1) {  // 게임 시작 시 인트로 출력
			startIntro = true;  // 카메라 애니메이션 활성화
			ui.intro = true;  //  ui 애니메이션 활성화
			ssystem->playSound(menuClick, 0, false, &channelMenu);
		}

		else if (gameStarted && !setInfo) {  // 게임 시작 이후에는 메뉴를 여는 기능을 한다.
			if (!ui.menuEnable)
				openMenu();
			else
				closeMenu();

			alcy.isLeave = false;
		}
		break;

	case 9:  // tab, 팁 키고 끄기
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
		if (gameStarted) {  // 양 방향 모두 쓰다듬지 않고 메뉴바와 기능 실행이 비활성화 된 상태에서만 사용 가능
			if (!alcy.touchEnable && !ui.menuEnable && !playFunc) {
				cam.camL = true;
				mouseClickEnable = false;  // 이 때 마우스 클릭을 할 수 없게 된다.
			}
			alcy.isLeave = false;
		}
		break;

	case 'e':  // 카메라 우측 회전
		if (gameStarted) {
			if (!alcy.touchEnable && !ui.menuEnable && !playFunc) {
				cam.camR = true;
				mouseClickEnable = false;
			}
			alcy.isLeave = false;
		}
		break;

	case 'i':  // info 보기
		if (!ui.menuEnable && !ui.exitEnable) {  // 나가기 밒 메뉴바 비활성화 상태에서만 사용 가능
			if (!escSoundPlayed) {  // 중복 재생 방지
				channelEscDown->stop();
				ssystem->playSound(escDown, 0, false, &channelEscDown);
				escSoundPlayed = true;
				ui.exitTransparent = 0.0;
			}
			
			if (!setInfo) {
				ExitOrInfo = 1;  // ui 클래스에서 정보창 기능으로 인식하게 된다.
				ui.infoEnable = true;  // info 활성화
			}
			if (!playFunc)
				initAlcyKey();  // 알키 쓰다듬기 중이라면 중단 후 초기화 한다.
		}
		alcy.isLeave = false;
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void keyUp(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // esc, 나가기
		if (!ui.menuEnable) {
			channelEscUp->stop();
			ssystem->playSound(escUp, 0, false, &channelEscUp);
			escSoundPlayed = false;
		}
		ui.exitEnable = false;  // 나가기 비활성화
		break;

	case 'q':  // 카메라 우측 회전 해제
		cam.camL = false;
		alcy.tiltSoundPlayed = false;  // 알키 머리 기울이기 소리를 다시 재생할 수 있게 된다.
		break;

	case 'e':  // 카메라 좌측 회전 해제
		cam.camR = false;
		alcy.tiltSoundPlayed = false;
		break;

	case 'i':  // info 
		if (!ui.menuEnable) {
			channelEscUp->stop();
			ssystem->playSound(escUp, 0, false, &channelEscUp);
			escSoundPlayed = false;
		}

		ui.infoEnable = false;  // 정보창 비활성화

		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
