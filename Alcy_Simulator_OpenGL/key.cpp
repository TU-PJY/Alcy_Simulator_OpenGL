// 키보드 조작
#include "gl_func.h"
#include "globalVar.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"

void keyDown(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:  // ESC
		if (!escSoundPlayed) {  // 중복 재생 방지
			channelEscDown->stop();
			ssystem->playSound(escDown, 0, false, &channelEscDown);
			escSoundPlayed = true;
		}
		ui.exitEnable = true;
		alcy.isLeave = false;
		break;

	case 9:  // tab
		channelClick->stop();
		ssystem->playSound(click, 0, false, &channelClick);

		if (!ui.tipEnable) {
			ui.tipEnable = true;
			break;
		}
		else 
			ui.tipEnable = false;
		alcy.isLeave = false;
		break;

	case 'q':  // 카메라 좌측 회전
		if (!alcy.touchEnable) {
			cam.camL = true;
			mouseClickEnable = false;
		}
		alcy.isLeave = false;
		break;

	case 'e':  // 카메라 우측 회전
		if (!alcy.touchEnable) {
			cam.camR = true;
			mouseClickEnable = false;
		}
		alcy.isLeave = false;
		break;

	case 'i':  // info 보기
		if (!escSoundPlayed) {  // 중복 재생 방지
			channelEscDown->stop();
			ssystem->playSound(escDown, 0, false, &channelEscDown);
			escSoundPlayed = true;
		}
		ui.infoEnable = true;  // info 활성화
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}

void keyUp(unsigned char KEY, int x, int y) {
	switch (KEY) {
	case 27:
		channelEscUp->stop();
		ssystem->playSound(escUp, 0, false, &channelEscUp);
		escSoundPlayed = false;

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
		channelEscUp->stop();
		ssystem->playSound(escUp, 0, false, &channelEscUp);
		escSoundPlayed = false;

		ui.infoEnable = false;
		break;
	}
	if (glutGetWindow() != 0)
		glutPostRedisplay();
}
