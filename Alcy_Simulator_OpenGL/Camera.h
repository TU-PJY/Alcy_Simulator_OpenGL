#ifndef CAMERA_H
#define CAMERA_H
#include "config.h"
#include "globalVar.h"
#include "sound.h"

// Alcy  <-- Camera -->  UI
class Camera {
public:
	GLfloat camX, camY;  // ���� ȭ�� ī�޶�
	GLfloat camRot;  // ī�޶� ȸ�� ����
	GLfloat zoom = 1.0;  // ī�޶� ��
	GLfloat zoomAcc;  // �� ���ӵ�
	bool zoomEnable;  // �� ����
	bool camR, camL;  // ī�޶� �¿� ȸ��
    bool startFirst;  // ���� �ð��� �����Ͽ� �ð� ���� �����Ѵ�.

    GLfloat beatAcc;

    GLfloat testfloat;
    GLfloat testacc;

    Camera() {
        if (INTRO == 0)
            zoom = 1.0;
        else if (INTRO == 1)
            zoom = 0.6;
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
        if (testfloat > -0.2) {
            testfloat -= testacc * fs;
            testacc += fs / 5;
            cout << testacc << endl;
        }  // 2.8

        if (zoomEnable) {
            zoom += zoomAcc * fs;

            if (zoomAcc > 0) {  // �� ��
                zoomAcc -= fs / 10;
                if (zoomAcc < 0 || zoom > 2.62) {  // �ִ� �� �� ���� �ö󰡸� �� ���� �ߴ��Ѵ�. 
                    zoomAcc = 0;
                    if (zoom > 2.62)
                        zoom = 2.62;
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

    void introAnimation() {
        if (startIntro) {
            zoom += fs / 30;
            if (zoom > 1.0) {
                zoom = 1.0;
                startIntro = false;
                gameStarted = true;
            }
        }
    }

    // ��Ʈ�γ� �Լ�
    void metronomeEffect(int track) {
        switch (track) {  // Ʈ���� ���� ���� ������ �ٸ��� �����Ѵ�.
        case 0: 
            interval = 4.688;  // 128bpm
            playTime = 317;  // 31.7sec
            break;
        }

        if (beatDelay >= interval) {
            beatAcc = 0.08;
            zoom = 1.15;
            interpolation = beatDelay;
            beatDelay = 0;
            beatDelay = interpolation - interval;  // �ʰ� �ð� ����
        }

        else {
            zoom -= beatAcc * fs;
            beatAcc -= fs / 50;
            if (zoom < 1.0)
                zoom = 1.0;
            if (beatAcc < 0)
                beatAcc = 0;
        }

        beatDelay += fs;
        functionOperationTime += fs;
       
        if (functionOperationTime > playTime) {  // 31.7sec
            playFunc = false; // �뷡 ���� 31.7sec ������ �뷡 ��� ���� ��Ȱ��ȭ
            beatDelay = 0;  // ���� ���� �ð� �ʱ�ȭ
            functionOperationTime = 0;
            whiteTransparent = 1.0;
            channelMusic->stop();  // �ٽ� ���� �׸����� ����Ѵ�.
            ssystem->playSound(mainTheme, 0, false, &channelTheme);
        }
    }
};

extern Camera cam;

#endif
