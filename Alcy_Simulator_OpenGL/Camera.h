#ifndef CAMERA_H
#define CAMERA_H
#include "config.h"
#include "globalVar.h"
// Alcy  <-- Camera -->  UI
class Camera {
public:
	GLfloat camX, camY;  // ���� ȭ�� ī�޶�
	GLfloat camRot;  // ī�޶� ȸ�� ����
	GLfloat zoom = 1.0;  // ī�޶� ��
	GLfloat zoomAcc;  // �� ���ӵ�
	bool zoomEnable;  // �� ����
	bool camR, camL;  // ī�޶� �¿� ȸ��

    Camera() {
        zoom = 1.0;
    }

    void rotateCam() {  // ī�޶� ȸ��
        if (camR) {  // ī�޶� ���� ȸ�� ��
            camRot -= 3 * fs;
            if (camRot < -10)
                camRot = -10;
        }

        if (camL) {  // ī�޶� ���� ȸ�� ��
            camRot += 3 * fs;
            if (camRot > 10)
                camRot = 10;
        }

        if (!camR && !camL) {  // ī�޶� ȸ�� ������ ���� ��
            if (camRot > 0) {
                camRot -= 3 * fs;
                if (camRot < 0) {
                    camRot = 0;
                    mouseClickEnable = true;  // ī�޶� ������ 0�� �Ǹ� �ٽ� Ŀ���� Ȱ��ȭ �ȴ�
                }
            }
            if (camRot < 0) {
                camRot += 3 * fs;
                if (camRot > 0) {
                    camRot = 0;
                    mouseClickEnable = true;
                }
            }
        }
    }

    void updateZoom() {  // ī�޶� ��
        if (zoomEnable) {
            zoom += zoomAcc * fs;

            if (zoomAcc > 0) {  // �� ��
                zoomAcc -= fs / 10;
                if (zoomAcc < 0 || zoom > 2.7) {  // �ִ� �� �� ���� �ö󰡸� �� ���� �ߴ��Ѵ�. 
                    zoomAcc = 0;
                    if (zoom > 2.7)
                        zoom = 2.7;
                    zoomEnable = false;
                }
            }

            else if (zoomAcc < 0) {  // �� �ƿ�
                zoomAcc += fs / 10;
                if (zoomAcc > 0 || zoom < 1.0) {  // �ּ� �� �� ������ �������� �� �ƿ��� �ߴ��Ѵ�.
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