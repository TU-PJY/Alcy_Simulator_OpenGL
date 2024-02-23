// 타이머
#include "gl_func.h"
#include "globalVar.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "ZZZ.h"
#include "Icon.h"
#include "White.h"

// 프레임
int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

void syncFrame() {  // 프레임 동기화
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 100.0; // Convert milliseconds to seconds
 
    
    lastElapsedTime = elapsedTime;
}

void syncCamera() {
    cam.camX = (0.0 - mx) / 10 / cam.zoom;
    cam.camY = (0.0 - my) / 10 / cam.zoom;
}

void syncAlcyHead() {
    if (mx / cam.zoom * ratio_ < -0.5 * ratio_)
        alcy.dir = l;
    else if (mx / cam.zoom * ratio_ > 0.5 * ratio_)
        alcy.dir = r;
    else
        alcy.dir = m;
}

void syncCursor() {
    if ((mx / cam.zoom * ratio_ >= -0.4 && mx / cam.zoom * ratio_ <= 0.4) && (my / cam.zoom >= 0.1 && my / cam.zoom <= 0.5)) // 쓰다듬기
        ui.handEnable = true;
    else
        ui.handEnable = false;

    if ((mx / cam.zoom * ratio_ >= -0.03 && mx / cam.zoom * ratio_ <= 0.03) && (my / cam.zoom >= -0.2 && my / cam.zoom <= -0.15))  // 코 누르기
        ui.fingerEnable = true;
    else
        ui.fingerEnable = false;
}

void syncIconSelection() { 
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

void checkIconSituation() {
    if(playFunc) {  // 기능 실행 중에는 해당 아이콘이 달라진다.
        for (int i = 0; i < ICON_PART; i++) {
            if (icon[i].operating)
                icon[i].operating = true;
        }
    }

    else {  // 기능 실행 중이 아니라면 원래의 아이콘을 출력한다.
        for (int i = 0; i < ICON_PART; i++) {
            icon[i].operating = false;
        }
    }
}

void timerOperation(int value) {
    syncFrame();

    cam.introAnimation();
    cam.rotateCam();
    cam.updateZoom();
    if (playFunc) {
        switch (musicTrack) {
        case 0:
            cam.metronomeEffect(musicTrack);
            break;
        }

        alcy.updateAlcyBeat();
    }

    if (gameStarted) {  // 마우스 이벤트를 실시간으로 처리하기 위한 더블 싱크
        syncCamera();
        if (!ui.menuEnable && !playFunc) {
            syncCursor();
            syncAlcyHead();
        }
        if (ui.menuOpened && ui.menuEnable)
            syncIconSelection();

        ui.updateMenu();

        if (!playFunc) {
            alcy.checkControl();
            alcy.updateAlcySleep();
        }
    }

    for (int i = 0; i < ICON_PART; i++) {
        icon[i].updateIcon();
        icon[i].updateOnCursor();
    }

    ui.startGame();
    ui.exitGame();
    ui.updateTip();
    alcy.updateAlcyBlink();
    alcy.moveAlcyHead();
    alcy.updateAlcyTouch();
    alcy.tiltAlcyHead();
    alcy.squeakAlcyNose();

    for(int i = 0; i < 3; i ++)
     zzz[i].update();

    white.update();

    checkIconSituation();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
