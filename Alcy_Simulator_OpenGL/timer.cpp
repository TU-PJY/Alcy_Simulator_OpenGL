// 타이머
#include "gl_func.h"

int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

extern GLfloat camRot;
extern bool camR, camL;

void syncFrame() {
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 10.0; // Convert milliseconds to seconds
    lastElapsedTime = elapsedTime;
}

void rotateCam() {
    if (camR) {  // 카메라 우측 회전 시
        camRot -= fs / 2;
        if (camRot < -15)
            camRot = -15;
    }

    if (camL) {  // 카메라 좌측 회전 시
        camRot += fs / 2;
        if (camRot > 15)
            camRot = 15;
    }

    if (!camR && !camL) {  // 카메라 회전 조작이 없을 시
        if (camRot > 0) {
            camRot -= fs / 2;
            if (camRot < 0)
                camRot = 0;
        }
        if (camRot < 0) {
            camRot += fs / 2;
            if (camRot > 0)
                camRot = 0;
        }
    }
}

void timerOperation(int value) {
    syncFrame();

    rotateCam();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
