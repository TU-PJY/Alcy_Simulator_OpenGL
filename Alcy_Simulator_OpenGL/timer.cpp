// 타이머
#include "gl_func.h"

// 프레임
int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

// 카메라
extern GLfloat camRot;
extern bool camR, camL;

// 커서
extern bool cursorEnable;

// 알키 바라보는 방향
extern int dir;

// 알키 머리 움직임
extern GLfloat headPos;

void syncFrame() {  // 프레임 동기화
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 10.0; // Convert milliseconds to seconds
    lastElapsedTime = elapsedTime;
}

void rotateCam() {  // 카메라 회전
    if (camR) {  // 카메라 우측 회전 시
        camRot -= fs / 2;
        if (camRot < -10)
            camRot = -10;
    }

    if (camL) {  // 카메라 좌측 회전 시
        camRot += fs / 2;
        if (camRot > 10)
            camRot = 10;
    }

    if (!camR && !camL) {  // 카메라 회전 조작이 없을 시
        if (camRot > 0) {
            camRot -= fs / 2;
            if (camRot < 0) {
                camRot = 0;
                cursorEnable = true;  // 카메라 각도가 0이 되면 다시 커서가 활성화 된다
            }
        }
        if (camRot < 0) {
            camRot += fs / 2;
            if (camRot > 0) {
                camRot = 0;
                cursorEnable = true;
            }
        }
    }
}

void moveAlcyHead() {  // 바라보는 방향 전환 시 알키 머리 움직임
    if (!camR && !camL && camRot == 0) {
        switch (dir) {
        case l:  // 좌측 바라볼 시
            headPos -= 0.003 * fs;
            if (headPos < -0.05)
                headPos = -0.05;
            break;

        case r:  // 우측 바라볼 시
            headPos += 0.003 * fs;
            if (headPos > 0.05)
                headPos = 0.05;
            break;

        default:  // 가운데 바라볼 시
            if (headPos < 0) {
                headPos += 0.003 * fs;
                if (headPos > 0)
                    headPos = 0;
            }
            if (headPos > 0) {
                headPos -= 0.003 * fs;
                if (headPos < 0)
                    headPos = 0;
            }
            break;
        }
    }
    else {
        if (headPos < 0) {
            headPos += 0.004 * fs;
            if (headPos > 0)
                headPos = 0;
        }
        if (headPos > 0) {
            headPos -= 0.004 * fs;
            if (headPos < 0)
                headPos = 0;
        }
    }
}

void timerOperation(int value) {
    syncFrame();

    rotateCam();
    moveAlcyHead();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
