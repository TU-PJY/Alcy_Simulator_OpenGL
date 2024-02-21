// 타이머
#include "gl_func.h"
#include "globalVar.h"
#include "Alcy.h"
#include "Camera.h"
#include "UI.h"
#include "ZZZ.h"

// 프레임
int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

void syncFrame() {  // 프레임 동기화
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 100.0; // Convert milliseconds to seconds
    lastElapsedTime = elapsedTime;
}

void timerOperation(int value) {
    syncFrame();

    cam.introAnimation();
    cam.rotateCam();
    cam.updateZoom();

    ui.startGame();
    ui.exitGame();
    ui.updateTip();

    if (gameStarted) {
        alcy.checkControl();
        alcy.updateAlcySleep();
    }
    alcy.updateAlcyBlink();
    alcy.moveAlcyHead();
    alcy.updateAlcyTouch();
    alcy.tiltAlcyHead();
    alcy.squeakAlcyNose();

    for(int i = 0; i < 3; i ++)
     zzz[i].update();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
