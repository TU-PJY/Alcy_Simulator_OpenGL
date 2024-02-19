// 타이머
#include "gl_func.h"
#include "gameVariable.h"

random_device rd;  mt19937 gen(rd());
uniform_real_distribution <GLfloat> dis(0, 3);

// 프레임
int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

void syncFrame() {  // 프레임 동기화
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 100.0; // Convert milliseconds to seconds
    lastElapsedTime = elapsedTime;
}

// ui

void exitGame() {  // esc를 길게 눌러 게임 종료
    if (exitEnable) {
        exitTransparent += fs / 6;
        if (exitTransparent > 1.0) {
            exitTransparent = 1.0;
            glutDestroyWindow(1);
        }
    }
    else {
        exitTransparent -= fs / 6;
        if (exitTransparent < 0) {
            exitTransparent = 0;
        }
    }
}

void rotateCam() {  // 카메라 회전
    if (camR) {  // 카메라 우측 회전 시
        camRot -= 3 * fs;
        if (camRot < -10) 
            camRot = -10;
    }

    if (camL) {  // 카메라 좌측 회전 시
        camRot += 3 * fs;
        if (camRot > 10) 
            camRot = 10;
    }

    if (!camR && !camL) {  // 카메라 회전 조작이 없을 시
        if (camRot > 0) {
            camRot -= 3 * fs;
            if (camRot < 0) {
                camRot = 0;
                cursorEnable = true;  // 카메라 각도가 0이 되면 다시 커서가 활성화 된다
            }
        }
        if (camRot < 0) {
            camRot += 3 * fs;
            if (camRot > 0) {
                camRot = 0;
                cursorEnable = true;
            }
        }
    }
}

void updateZoom() {  // 카메라 줌
    if (zoomEnable) {
        zoom += zoomAcc * fs;

        if (zoomAcc > 0) {  // 줌 인
            zoomAcc -= fs / 10;
            if (zoomAcc < 0 || zoom > 2.7) {  // 최대 줌 값 위로 올라가면 줌 인을 중단한다. 
                zoomAcc = 0;
                if (zoom > 2.7)
                    zoom = 2.7;
                zoomEnable = false;
            }
        }

        else if (zoomAcc < 0) {  // 줌 아웃
            zoomAcc += fs / 10;
            if (zoomAcc > 0 || zoom < 1.0) {  // 최소 줌 값 밑으로 내려가면 줌 아웃을 중단한다.
                zoomAcc = 0;
                if (zoom < 1.0)
                    zoom = 1.0;
                zoomEnable = false;
            }
        }
    }
}

void updateTip() {  // 팁 on/off
    if (tipEnable) {
        tipTransparent += fs / 3;
        if (tipTransparent > 1.0)
            tipTransparent = 1.0;
    }
    else {
        tipTransparent -= fs / 3;
        if (tipTransparent < 0.0)
            tipTransparent = 0.0;
    }
}


// in game

void updateAlcyBlink() {  // 알키 눈 깜빡임 업데이트
    if (!blinkEnable) {
        endTime = time(NULL);  // 시간을 실시간으로 측정하여 아래의 조건에 맞는지 검사한다
        if (GLfloat(endTime - startTime) > blinkInterval)  // 지정된 간격보다 시간이 지나면 눈 깜빡임이 활성화 된다.
            blinkEnable = true;
    }

    else {
        keepTimer += fs;  // 아주 짧은 시간 동안 눈 감은 상태를 유지하다가 다시 눈을 뜬다.
        if (keepTimer > 1.5) {
            startTime = time(NULL);
            keepTimer = 0;
            blinkInterval = dis(gen);  // 눈 깜빡이는 간격을 랜덤으로 설정한다
            blinkEnable = false;
        }
    }
}

void moveAlcyHead() {  // 바라보는 방향 전환 시 알키 머리 움직임
    if (!camR && !camL && camRot == 0) {
        switch (dir) {
        case l:  // 좌측 바라볼 시
            headPos -= 0.03 * fs;
            if (headPos < -0.05)
                headPos = -0.05;
            break;

        case r:  // 우측 바라볼 시
            headPos += 0.03 * fs;
            if (headPos > 0.05)
                headPos = 0.05;
            break;

        default:  // 가운데 바라볼 시
            if (headPos < 0) {
                headPos += 0.03 * fs;
                if (headPos > 0)
                    headPos = 0;
            }
            if (headPos > 0) {
                headPos -= 0.03 * fs;
                if (headPos < 0)
                    headPos = 0;
            }
            break;
        }
    }
    else {  // 아무 조작 없을 때
        if (headPos < 0) {
            headPos += 0.03 * fs;
            if (headPos > 0)
                headPos = 0;
        }
        if (headPos > 0) {
            headPos -= 0.03 * fs;
            if (headPos < 0)
                headPos = 0;
        }
    }
}

void updateAlcyTouch() {  // 알키 머리 쓰다듬기
    if (cursorEnable && handEnable && touchEnable) {  // 손 커서인 상태로 머리를 쓰다듬을 수 있다
        handX = sin(handNum) * 0.2;  // 쓰다듬는 중에는 손 커서가 좌우로 부드럽게 움직인다.
        tailRot = sin(tailNum) * 10;
        handNum += fs / 4;
        tailNum += fs / 10;  // 꼬리는 느린 속도로 별도로 움직인다.
        headRot = -handX * 25;  // 손의 움직임에 따라 머리도 같이 움직인다.
        bodyRot = -headRot / 4;  // 몸통도 같이 움직인다.
    }
}

void tiltAlcyHead() {
    if (camRot < -9.9) {  // 카메라가 완전히 기울어진 후  알키가 머리를 기울인다.
        headTiltL = false;
        headTiltR = true;
    }

    if (camRot > 9.9) {
        headTiltL = true;
        headTiltR = false;
    }

    if (headTiltR) {  // 머리를 오른쪽으로 기울일경우
        headRot -= fs * 2;
        tailRot -= fs * 2;
        if (headRot < -10) {
            headRot = -10;
            tailRot = -10;
        }
    }

    if (headTiltL) { // 머리를 왼쪽으로 기울일경우
        headRot += fs * 2;
        tailRot += fs * 2;
        if (headRot > 10) {
            headRot = 10;
            tailRot = 10;
        }
    }
    
    if(camRot == 0 && !camR && !camL && !touchEnable) {  // 카메라가 처음으로 되돌아가면 머리를 다시 가운데로 세운다.
        headTiltR = false;
        headTiltL = false;

        if (headRot > 0) {
            headRot -= fs * 2;
            tailRot -= fs * 2;
            if (headRot < 0) {
                headRot = 0;
                tailRot = 0;
            }
        }
        if (headRot < 0) {
            headRot += fs * 2;
            tailRot += fs * 2;
            if (headRot > 0) {
                headRot = 0;
                tailRot = 0;
            }
        }
    }
}


void timerOperation(int value) {
    syncFrame();

    exitGame();
    rotateCam();
    updateZoom();
    updateTip();

    updateAlcyBlink();
    moveAlcyHead();
    updateAlcyTouch();
    tiltAlcyHead();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
