#ifndef CAMERA_H
#define CAMERA_H
#include "config.h"
#include "globalVar.h"
#include "sound.h"

// Alcy  <-- Camera -->  UI
class Camera {
public:
	GLfloat camX, camY;  // 게임 화면 카메라
	GLfloat camRot;  // 카메라 회전 각도
	GLfloat zoom = 1.0;  // 카메라 줌
	GLfloat zoomAcc;  // 줌 가속도
	bool zoomEnable;  // 줌 여부
	bool camR, camL;  // 카메라 좌우 회전
    bool startFirst;  // 이전 시간을 측정하여 시간 차를 보간한다.

    GLfloat beatAcc;

    GLfloat testfloat;
    GLfloat testacc;

    Camera() {
        if (INTRO == 0)
            zoom = 1.0;
        else if (INTRO == 1)
            zoom = 0.6;
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
                    mouseClickEnable = true;  // 카메라 각도가 0이 되면 다시 커서가 활성화 된다
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

    void updateZoom() {  // 카메라 줌
        if (testfloat > -0.2) {
            testfloat -= testacc * fs;
            testacc += fs / 5;
            cout << testacc << endl;
        }  // 2.8

        if (zoomEnable) {
            zoom += zoomAcc * fs;

            if (zoomAcc > 0) {  // 줌 인
                zoomAcc -= fs / 10;
                if (zoomAcc < 0 || zoom > 2.62) {  // 최대 줌 값 위로 올라가면 줌 인을 중단한다. 
                    zoomAcc = 0;
                    if (zoom > 2.62)
                        zoom = 2.62;
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

    // 메트로놈 함수
    void metronomeEffect(int track) {
        switch (track) {  // 트랙에 따라 박자 간격을 다르게 설정한다.
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
            beatDelay = interpolation - interval;  // 초과 시간 보간
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
            playFunc = false; // 노래 길이 31.7sec 지나면 노래 재생 상태 비활성화
            beatDelay = 0;  // 박자 지연 시간 초기화
            functionOperationTime = 0;
            whiteTransparent = 1.0;
            channelMusic->stop();  // 다시 메인 테마곡을 재생한다.
            ssystem->playSound(mainTheme, 0, false, &channelTheme);
        }
    }
};

extern Camera cam;

#endif
