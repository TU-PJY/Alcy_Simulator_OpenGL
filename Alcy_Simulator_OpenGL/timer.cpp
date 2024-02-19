// 타이머
#include "gl_func.h"

random_device rd;  mt19937 gen(rd());
uniform_real_distribution <GLfloat> dis(0, 3);

// 일시정지
extern bool pause;
extern GLfloat alcyHeight;
extern GLfloat pauseAcc;

// 프레임
int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

// 카메라
extern GLfloat camRot;
extern bool camR, camL;

// 커서
extern bool cursorEnable;

// 이미지 투명도
extern GLfloat transparent;

// 알키 관련 변수
extern int dir; // 알키 바라보는 방향
extern GLfloat headPos; // 알키 머리 움직임
extern time_t startTime, endTime, blinkTime; // 눈 깜빡임 타이머
extern GLfloat blinkInterval; // 눈 깜빡임 간격
extern GLfloat keepTimer; // 눈 감은 상태를 유지한다
extern bool blinkEnable;  // 눈 깜빡임 여부

extern GLfloat zoomAcc, zoom;
extern bool zoomEnable;


void syncFrame() {  // 프레임 동기화
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 100.0; // Convert milliseconds to seconds
    lastElapsedTime = elapsedTime;
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
    else {
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

void updateAlcyBlink() {  // 알키 눈 깜빡임 업데이트
    if (!blinkEnable) {
        endTime = time(NULL);  // 시간을 실시간으로 측정하여 아래의 조건에 맞는지 검사한다
        if (GLfloat(endTime - startTime) > blinkInterval) {  // 지정된 간격보다 시간이 지나면 눈 깜빡임이 활성화 된다.
            blinkEnable = true;
        }
    }

    else {
        keepTimer += fs;  // 아주 짧은 시간 동안 눈 감은 상태를 유지하다가 다시 눈을 뜬다.
        if (keepTimer > 1.5) {
            startTime = time(NULL);
            keepTimer = 0;
            blinkInterval = dis(gen);  // 눈 깜빡이는 시간을 랜덤으로 설정한다
            blinkEnable = false;
        }
    }
}

void updateZoom() {  // 카메라 줌
    if (zoomEnable) {
        zoom += zoomAcc * fs;

        if (zoomAcc > 0) {  // 줌 인
            zoomAcc -= fs / 10;
            if (zoomAcc < 0 || zoom > 3.0) {  // 최대 줌 값 위로 올라가면 줌 인을 중단한다. 
                zoomAcc = 0;
                if (zoom > 3.0)
                    zoom = 3.0;
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

void updatePause() {
    if (pause) {  // 일시정지 활성화
        alcyHeight += pauseAcc * fs;
        pauseAcc -= fs / 10;
        if (pauseAcc < 0) {
            pauseAcc = 0;
        }
        cout << alcyHeight << endl;
    }

    else if (!pause) {  // 일시정지 비활성화
        alcyHeight -= pauseAcc * fs;
        pauseAcc -= fs / 10;
        if (pauseAcc < 0) {
            alcyHeight = 0;
            pauseAcc = 0;
        }
    }
}

void timerOperation(int value) {
    syncFrame();

    rotateCam();
    moveAlcyHead();
    updateAlcyBlink();
    updateZoom();
    updatePause();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
