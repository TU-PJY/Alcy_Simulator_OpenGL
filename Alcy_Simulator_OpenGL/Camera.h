#ifndef CAMERA_H
#define CAMERA_H
#include "config.h"
#include "gameVariable.h"

class Camera {
public:
	GLfloat camX, camY;  // 게임 화면 카메라
	GLfloat camRot;  // 카메라 회전 각도
	GLfloat zoom = 1.0;  // 카메라 줌
	GLfloat zoomAcc;  // 줌 가속도
	bool zoomEnable;  // 줌 여부
	bool camR, camL;  // 카메라 좌우 회전

    Camera() {
        zoom = 1.0;
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
};

extern Camera cam;

#endif
