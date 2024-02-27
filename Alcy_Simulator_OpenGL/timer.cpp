// 타이머
#include "gl_func.h"
#include "globalVar.h"
#include "Background.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "ZZZ.h"
#include "Icon.h"
#include "White.h"
#include "speaker.h"
#include "Turntable.h"
#include "Guitar.h"
#include "Arm.h"
#include "Light.h"
#include "Note.h"
#include "Info.h"
#include "Start.h"

// 프레임
int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

void syncFrame() {  // 프레임 동기화
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 100.0; // Convert milliseconds to seconds(0.1sec unit, ex: 31.1s = GLfloat(311))
    lastElapsedTime = elapsedTime;
}

void updateCamera() {
    cam.camX = (0.0 - mx) / 10 / cam.zoom;
    cam.camY = (0.0 - my) / 10 / cam.zoom;
}

void updateAlcyHeadDir() {
    if (mx / cam.zoom * ratio_ < -0.5 * ratio_)
        alcy.dir = l;
    else if (mx / cam.zoom * ratio_ > 0.5 * ratio_)
        alcy.dir = r;
    else
        alcy.dir = m;
}

void updateCursorType() {
    if ((mx / cam.zoom * ratio_ >= -0.4 && mx / cam.zoom * ratio_ <= 0.4) && (my / cam.zoom >= 0.1 && my / cam.zoom <= 0.5)) // 쓰다듬기
        ui.handEnable = true;
    else
        ui.handEnable = false;

    if ((mx / cam.zoom * ratio_ >= -0.03 && mx / cam.zoom * ratio_ <= 0.03) && (my / cam.zoom >= -0.2 && my / cam.zoom <= -0.15))  // 코 누르기
        ui.fingerEnable = true;
    else
        ui.fingerEnable = false;
}

void checkIsOnCursorIcon() { 
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

void updateIconType() {
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

void setFuncRunningTime(int num) {
    switch (num) {
    case 0:
        interval = 4.688;  // 128bpm
        playTime = 317;  // 31.7sec
        break;
    case 1:
        interval = 5.455;  // 110bpm
        playTime = 351;  //35.1sec
        break;
    case 2:
        interval = 0;
        playTime = 213;
        break;
    case 3:
        interval = 0;
        playTime = 200;
        break;
    }
}

void initAlcyFunc() {
    alcy.beatX = 0;
    alcy.beatY = 0;
    alcy.headPos = 0;
    alcy.headPosY = 0;
    alcy.headRot = 0;
    alcy.headNum = 0;
    alcy.tailRot = 0;
    alcy.tailNum = 0;
    alcy.sleepHeight = 0;
}

void updateBeatDelay() {
    if (beatDelay >= interval) {
        beatAcc = 0.077;
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
}

// 메뉴바 기능 실행 함수
void updateFuncOperation(int num) {
    setFuncRunningTime(num);

    if (num == 0 || num == 1)
        updateBeatDelay();

    functionOperationTime += fs;

    if (functionOperationTime > playTime) { 
        playFunc = false;

        cam.zoom = 1.0;
        cam.camRot = 0;

        initAlcyFunc();

        beatDelay = 0;  // 박자 지연 시간 초기화
        beatVal = 0;

        functionOperationTime = 0;
        whiteTransparent = 1.0;

        for (int i = 0; i < ICON_PART; i++) {  // 아이콘 효과 초기화
            icon[i].iconBeatRot = 0;
            icon[i].iconBeatX = 0;
            icon[i].operating = false;
        }

        channelMusic->stop();  // 다시 메인 테마곡을 재생한다.
        ssystem->playSound(mainTheme, 0, false, &channelTheme);
    }
}

void timerOperation(int value) {
    syncFrame();

    if (INTRO == 1) {
        if (!gameStarted) {  // 게임 로딩 부분
            if (!LoadingEnd)
                start.loading();
            if (LoadingEnd)  // 로딩 이후부터 아래 타이머 코드 실행 시작
                start.updateLoadingDisplay();  // 로딩 화면 업데이트
        }
    }

    if (LoadingEnd) {  // 로딩 시작 이후부터 리소스 타이머 작업 시작
        if (ui.intro)
            ui.startGame();

        if (!gameStarted)
            cam.introAnimation();

        if (gameStarted) { // 게임 시작 후에만 동작하는 코드 모음
            //카메라
            updateCamera(); // 마우스 이벤트를 실시간으로 처리하기 위한 더블 싱크
            cam.rotateCam();
            cam.updateZoom();

            // 커서
            if (!ui.menuEnable && !playFunc && !setInfo && !ui.exitEnable && !ui.infoEnable)
                updateCursorType();

            // 메뉴바
            ui.updateMenu();

            // 팁
            ui.updateTip();

            // 아이콘
            for (int i = 0; i < ICON_PART; i++) {
                icon[i].updateIcon();
                icon[i].updateOnCursor();
            }
            if (ui.menuOpened && ui.menuEnable)
                checkIsOnCursorIcon();
            updateIconType();

            // 알키
            if (!playFunc) {
                updateAlcyHeadDir();
                alcy.moveAlcyHead();
                alcy.updateAlcyTouch();
                alcy.tiltAlcyHead();
                alcy.squeakAlcyNose();
                alcy.checkControl();
                alcy.updateAlcySleep();
            }

            //알키
            if (playFunc) {
                alcy.updateAlcyBeat();
                cam.updateCameraBeat();
                speaker.updateSpeakerBeat();

                switch (funcNumber) {
                case 0:
                    light.updateLightTransparent();
                    updateFuncOperation(funcNumber);
                    break;
                case 1:
                    background.updateBackgroundIdx();
                    ;                turntable.updateTurntableIndex();  // 턴테이블의 색상이 바뀌어야 하므로 인덱스를 계속 업데이트 한다.
                    speaker.updateImageIndex();
                    alcy.updateImageIndex();
                    updateFuncOperation(funcNumber);
                    break;
                case 2:
                    arm.update();
                    guitar.update();
                    alcy.updateAlcyGuitarPlay();
                    note.update();
                    updateFuncOperation(funcNumber);
                    break;
                case 3:
                    arm.update();
                    guitar.update();
                    alcy.updateAlcyGuitarPlay();
                    updateFuncOperation(funcNumber);
                    break;
                }
            }

            // zzz 오브젝트
            for (int i = 0; i < 3; i++)
                zzz[i].update();

            // 흰배경
            if (whiteTransparent > 0.0)
                white.update();
        }

        // 게임 시작 여부 상관없이 항상 동작하는 코드 모음
        ui.exitGame();
        info.update();
        alcy.updateAlcyBlink();
    }

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
