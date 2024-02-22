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

void syncCamera() {
    cam.camX = (0.0 - mx) / 10 / cam.zoom;
    cam.camY = (0.0 - my) / 10 / cam.zoom;
}

void syncAlcyHead() {
    if (mx / cam.zoom * ratio < -0.5 * ratio)
        alcy.dir = l;
    else if (mx / cam.zoom * ratio > 0.5 * ratio)
        alcy.dir = r;
    else
        alcy.dir = m;
}

void syncCursor() {
    if ((mx / cam.zoom * ratio >= -0.4 && mx / cam.zoom * ratio <= 0.4) && (my / cam.zoom >= 0.1 && my / cam.zoom <= 0.5)) // 쓰다듬기
        ui.handEnable = true;
    else
        ui.handEnable = false;

    if ((mx / cam.zoom * ratio >= -0.03 && mx / cam.zoom * ratio <= 0.05) && (my / cam.zoom >= -0.2 && my / cam.zoom <= -0.15))  // 코 누르기
        ui.fingerEnable = true;
    else
        ui.fingerEnable = false;
}

void timerOperation(int value) {
    syncFrame();
    if (gameStarted) {  // 마우스 이벤트를 실시간으로 처리하기 위한 더블 싱크
        syncCamera();
        syncAlcyHead();
        syncCursor();
    }

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
