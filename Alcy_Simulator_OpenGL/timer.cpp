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

// 메트로놈 함수
void metronomeEffect(int track) {
    switch (track) {  // 트랙에 따라 박자 간격을 다르게 설정한다.
    case 0:
        interval = 4.688;  // 128bpm
        playTime = 317;  // 31.7sec
        break;
    }

    if (beatDelay >= interval) {
        beatAcc = 0.08;
        beatVal = 0.15;
        interpolation = beatDelay;
        beatDelay = 0;
        beatDelay = interpolation - interval;  // 초과 시간 보간
    }

    else {
        beatVal -= beatAcc * fs;
        beatAcc -= fs / 50;
        if (beatVal < 0.0)
            beatVal = 0.0;
        if (beatAcc < 0)
            beatAcc = 0;
    }

    beatDelay += fs;
    functionOperationTime += fs;

    if (functionOperationTime > playTime) {  // 31.7sec
        playFunc = false; // 노래 길이 31.7sec 지나면 노래 재생 상태 비활성화
        beatDelay = 0;  // 박자 지연 시간 초기화
        beatVal = 0;
        functionOperationTime = 0;
        whiteTransparent = 1.0;
        channelMusic->stop();  // 다시 메인 테마곡을 재생한다.
        ssystem->playSound(mainTheme, 0, false, &channelTheme);
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
            metronomeEffect(musicTrack);
            break;
        }

        alcy.updateAlcyBeat();
        cam.updateCameraBeat();
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
