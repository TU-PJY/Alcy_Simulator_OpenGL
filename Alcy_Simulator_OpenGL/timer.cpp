// 타이머
#include "gl_func.h"
#include "gameVariable.h"
#include "Alcy.h"

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


void timerOperation(int value) {
    syncFrame();

    exitGame();
    rotateCam();
    updateZoom();
    updateTip();

    alcy.updateAlcyBlink();
    alcy.moveAlcyHead();
    alcy.updateAlcyTouch();
    alcy.tiltAlcyHead();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
