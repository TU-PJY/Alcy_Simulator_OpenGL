// 타이머
#include "gl_func.h"
#include "gameVariable.h"
#include "Alcy.h"
#include "Camera.h"

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

    cam.rotateCam();
    cam.updateZoom();

    exitGame();
    updateTip();

    alcy.updateAlcyBlink();
    alcy.moveAlcyHead();
    alcy.updateAlcyTouch();
    alcy.tiltAlcyHead();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
