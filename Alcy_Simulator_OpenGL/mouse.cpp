#include "gl_func.h"
#include "globalVar.h"
#include "screen.h"
#include "sound.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"

void convert_to_gl(int x, int y) {  //GL좌표계로 변환
	mx = (GLfloat)(x - (GLfloat)WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(WIDTH / 2.0));
	my = -(GLfloat)(y - (GLfloat)HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(HEIGHT / 2.0));
}

void setDir() {  // 알키 바라보는 방향을 결정한다
	if (mx * ratio < -0.5 * ratio)
		alcy.dir = l;
	else if (mx * ratio > 0.5 * ratio)
		alcy.dir = r;
	else
		alcy.dir = m;
}

void updateCursor() {  // 알키 머리에 커서를 가져다대면 커서가 바뀐다
	if ((mx * ratio >= -0.4 && mx * ratio <= 0.4) && (my >= 0.1 && my <= 0.5)) // 쓰다듬기
		ui.handEnable = true;
	else
		ui.handEnable = false;

	if ((mx * ratio >= -0.03 && mx * ratio <= 0.05) && (my >= -0.2 && my <= -0.15))  // 코 누르가
		ui.fingerEnable = true;
	else
		ui.fingerEnable = false;
}

void Mouse(int button, int state, int x, int y) {  // 마우스 클릭
	int randomSound = 0;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (ui.handEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak) {
			channelTouch->stop();
			ssystem->playSound(touch, 0, false, &channelTouch);
			alcy.touchEnable = true;
			lButtonDown = true;
		}

		if (ui.fingerEnable && mouseClickEnable && cam.camRot == 0 && !alcy.squeak) {
			random_device rd;  mt19937 gen(rd());
			uniform_int_distribution <int> dis(1, 3);
			randomSound = dis(gen);

			channelSqueak->stop();

			switch(randomSound) {  // 3가지 중 하나를 재생한다.
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
			
			alcy.squeakStartTime = time(NULL);
			alcy.squeak = true;
			lButtonDown = true;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (ui.handEnable && mouseClickEnable && alcy.touchEnable) {
			ui.handNum = 0;
			alcy.headRot = 0;
			alcy.tailNum = 0;
			alcy.tailRot = 0;
			alcy.bodyRot = 0;
			alcy.touchEnable = false;
			channelTouch->stop();
		}
		lButtonDown = false;
	}
	glutPostRedisplay();
}

void pMotion(int x, int y) {  // 클릭 안할 때의 모션
		convert_to_gl(x, y);
		cam.camX = (0.0 - mx) / 10;
		cam.camY = (0.0 - my) / 10;

		setDir();
		updateCursor();

	glutPostRedisplay();
}

void Motion(int x, int y) {  // 클릭 할 때의 모션
	if (!alcy.touchEnable) {  // 알키를 쓰다듬는 도중에는 마우스 업데이트를 하지 않는다.
		convert_to_gl(x, y);
		cam.camX = (0.0 - mx) / 10;
		cam.camY = (0.0 - my) / 10;

		setDir();
		updateCursor();
	}

	glutPostRedisplay();
}

void Wheel(int button, int dir, int x, int y) {  // 마우스 휠
	if (dir > 0) {
		channelScroll->stop();
		ssystem->playSound(scroll, 0, false, &channelScroll);

		cam.zoomAcc = 0.2 + (cam.zoom - 1.0) / 10;
		cam.zoomEnable = true;
	}
	else if (dir < 0) {
		channelScroll->stop();
		ssystem->playSound(scroll, 0, false, &channelScroll);

		if (cam.zoom > 1.0) {
			cam.zoomAcc = -0.2 - (cam.zoom - 1.0) / 15;
			cam.zoomEnable = true;
		}
	}
	
	return;
}