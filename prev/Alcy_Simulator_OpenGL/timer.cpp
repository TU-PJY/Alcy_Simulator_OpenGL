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

void syncFrame() {  // 프레임 동기화
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 100.0; // Convert milliseconds to seconds(0.1sec unit, ex: 31.1s = GLfloat(311))
    lastElapsedTime = elapsedTime;
}

void updateCamera() {  // 카메라 업데이트
    cam.camX = (0.0 - mx) / 10 / cam.zoom;
    cam.camY = (0.0 - my) / 10 / cam.zoom;
}

void updateAlcyHeadDir() {  // 커서 위치에 따른 알키 머리 방향 업데이트
    if (mx / cam.zoom * ratio_ < -0.5 * ratio_)  // 좌측
        alcy.dir = l;
    else if (mx / cam.zoom * ratio_ > 0.5 * ratio_)  // 우측
        alcy.dir = r;
    else
        alcy.dir = m;  // 중앙, 기본값
}

void updateCursorType() {  // 커서 이미지 업데이트
    if ((mx / cam.zoom * ratio_ >= -0.4 && mx / cam.zoom * ratio_ <= 0.4) && (my / cam.zoom >= 0.1 && my / cam.zoom <= 0.5)) // 쓰다듬기
        ui.handEnable = true;  // 손 모양 커서로 바뀐다
    else
        ui.handEnable = false;

    if ((mx / cam.zoom * ratio_ >= -0.03 && mx / cam.zoom * ratio_ <= 0.03) && (my / cam.zoom >= -0.2 && my / cam.zoom <= -0.15))  // 코 누르기
        ui.fingerEnable = true;  // 손가락 모양 커서로 바뀐다
    else
        ui.fingerEnable = false;
}

void checkIsOnCursorIcon() { // 아이콘 위에 커서가 위치하는지 검사하는 함수
    for (int i = 0; i < ICON_PART; i++) {
        GLfloat iconX = (-0.53 + i * 0.35);  // 각 아이콘의 x 좌표
        GLfloat iconY = -0.33;  // 각 아이콘의 y 좌표
        if ((iconX - 0.14 <= mx * ratio_ && mx * ratio_ <= iconX + 0.14) &&
            (iconY - 0.13 <= my && my <= iconY + 0.13)) {
            icon[i].isOnCursor = true;  // 아이콘 위에 커서를 올리면 아이콘이 살짝 투명해진다.
        }
        else {
            icon[i].isOnCursor = false;  // 그 외의 아이콘은 다시 불투명해지거나 반응하지 않는다.
        }
    }
}

void updateIconType() {  // 아이콘 이미지 업데이트
    if(playFunc) {  // 기능 실행 중에는 해당하는 아이콘이 중지 아이콘으로 바뀐다.
        for (int i = 0; i < ICON_PART; i++) {
            if (icon[i].operating)
                icon[i].operating = true;  // 이 값이 활성화 되면 아이콘이 중지 아이콘으로 출력된다.
        }
    }

    else {  // 기능 실행 중이 아니라면 원래의 아이콘을 출력한다.
        for (int i = 0; i < ICON_PART; i++) {
            icon[i].operating = false;
        }
    }
}

void setFuncRunningTime(int num) {  // 박자 효과, 기능 실행 시간 지정 함수
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

void initAlcyTimer() { // 알키 상태 초기화
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

void updateBeatDelay() {  // 박자 효과 업데이트
    if (beatDelay >= interval) {  // 박자 딜레이 값이 지정된 간격값에 도달하면
        beatAcc = 0.077;  // 박자 효과 값 초기화
        beatVal = 0.15;  // 이 값은 여러 클래스에서 참조한다.
        interpolation = beatDelay;  // 도달한 순간의 박자 딜레이 값을 저장한다.
        beatDelay = 0;  // 박자 딜레이 초기화
        beatDelay = interpolation - interval;  // 지정된 간격값을 초과한 만큼의 값을 박자 딜레이에 더하여 박자 효과 지연을 보간한다.
    }

    else {  // 박자 딜레이 값이 도달하지 않는 동안에는 박자 효과 값을 업데이트 한다.
        beatVal -= beatAcc * fs;
        beatAcc -= fs / 50;
        if (beatVal < 0.0)
            beatVal = 0.0;
        if (beatAcc < 0)
            beatAcc = 0;
    }

    beatDelay += fs;  // 박자 딜레이를 계속 증가 시킨다.
}

void updateFuncOperation(int num) { // 메뉴바 기능 실행 함수
    setFuncRunningTime(num);  // 실행시간 및 박자 효과 간격 지정

    if (num == 0 || num == 1)  // 일부 기능 활성화 상태일 때만 박자 효과를 업데이트 한다.
        updateBeatDelay();

    functionOperationTime += fs;  // 기능 실행 시간 증가

    if (functionOperationTime > playTime) { // 지정된 시간을 초과하면 기능이 종료된다.
        playFunc = false;  // 기능 실행 비활성화

        cam.zoom = 1.0;  // 카메라 초기화
        cam.camRot = 0;

        initAlcyTimer();  // 알키 초기화

        beatDelay = 0;  // 박자 지연 시간 초기화
        beatVal = 0;  // 박자 효과 값 초기화

        functionOperationTime = 0;
        whiteTransparent = 1.0;  // 흰 배경 투명도 초기화

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

    if (INTRO == 1) { // 게임 실행 직후 로고 인트로를 실행한다.
        if (!gameStarted) {  // 게임 로딩 부분
            if (!LoadingEnd)
                start.loading();  // 로딩이 끝나지 않았다면 로딩을 시작한다.
            if (LoadingEnd) 
                start.updateLoadingDisplay();  // 로딩이 완료되면 로고 화면을 투명화 한다.
        }
    }

    if (LoadingEnd) {  // 로딩 시작 이후부터 인게임 타이머 작업 시작
        if (ui.intro)
            ui.startGame();  // 타이틀 애니메이션 업데이트

        if (!gameStarted)
            cam.introAnimation();  // 카메라 애니메이션 업데이트

        if (gameStarted) { // 게임 시작 후에만 동작하는 코드 모음
            //카메라
            updateCamera();
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

            // 알키, 기능 실행 비활성화 상태
            if (!playFunc) {
                updateAlcyHeadDir();
                alcy.moveAlcyHead();
                alcy.updateAlcyTouch();
                alcy.tiltAlcyHead();
                alcy.squeakAlcyNose();
                alcy.checkControl();
                alcy.updateAlcySleep();
            }

            //알키, 기능 실행 활성화 상태
            if (playFunc) {
                switch (funcNumber) {  // 기능 번호에 따라 업데이트하는 요소가 달라진다.
                case 0:
                    cam.updateCameraBeat();  // 카메라 박자 효과 업데이트
                    alcy.updateAlcyBeat();  // 알키 박자 효과 업데이트
                    speaker.updateSpeakerBeat();  // 스피커 박자 효과 업데이트
                    light.updateLightTransparent(); // 배경 이미지 투명도 업데이트
                    updateFuncOperation(funcNumber);
                    break;

                case 1:
                    cam.updateCameraBeat();  // 카메라 박자 효과 업데이트
                    alcy.updateAlcyBeat();  // 알키 박자 효과 업데이트
                    background.updateBackgroundIdx();  // 배경 이미지 인덱스 업데이트
                    turntable.updateTurntableIndex();  // 턴테이블의 이미지 인덱스 업데이트
                    speaker.updateImageIndex();  // 스피커 이미지 인덱스 업데이트
                    alcy.updateImageIndex();  // 알키 이미지 인덱스 업데이트
                    updateFuncOperation(funcNumber);
                    break;

                case 2:
                    arm.update();  // 팔, 기타 애니메이션 업데이트
                    guitar.update();
                    alcy.updateAlcyGuitarPlay();  // 알키 기타 연주 애니메이션 업데이트
                    note.update();  // 음표 오브젝트 애니메이션 업데이트
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
        ui.exitGame();  // 게임 종료 또는 정보창 기능 업데이트
        info.update();  // 정보창 애니메이션 업데이트
        alcy.updateAlcyBlink();  // 알키 눈 깜빡임 업데이트
    }

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
