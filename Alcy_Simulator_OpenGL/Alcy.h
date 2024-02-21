#ifndef ALCY_H
#define ALCY_H
#include "config.h"
#include "shader.h"
#include "buffer.h"
#include "transform.h"
#include "texture.h"
#include "screen.h"
#include "globalVar.h"
#include "sound.h"
#include "Camera.h"
#include "UI.h"

enum dir {  // 알키 바라보는 방향
    l, m, r
};

enum name {  // 각 이미지 출력 케이스 네임
    tail_,
    body_,
    hair_,
    head_,
    eye_,
    dot_,
    brow_,
    blink_,
};

enum breathe {
    in_, out_,
};

class Alcy {
public:
	int dir;  // 알키 바라보는 방향, 초기값 m

	bool blinkEnable; // 눈 깜빡임 여부, true일 시 깜빡임 활성화
	time_t startTime, endTime, blinkTime;  // 눈 깜빡임 간격 타이머
	GLfloat blinkInterval;  // 눈 깜빡임 간격, 기본 3초
	GLfloat keepTimer;  // 눈을 감은 상태를 아주 짧게 유지한다
	bool touchEnable;  // 알키 머리 쓰다듬기 여부

	//알키 머리, 얼굴 파츠 움직임
	GLfloat headPos;  // 방향 전환 시 전환한 방향으로 움직임
	GLfloat headRot;  // 쓰다듬기 시 머리 회전 각도
	GLfloat tailRot;  // 쓰다듬기 시 꼬리 회전 각도
	GLfloat bodyRot;  // 쓰다듬기 시 몸통 회전 각도
	GLfloat tailNum; // 꼬리 회전에 사용되는 수치
	bool headTiltR, headTiltL;  // 카메라 기울였을 때 알키 머리 기울이기 여부
    bool tiltSoundPlayed;  // 알키 머리 기울이는 소리 재생 여부, 중복 재생 방지

    bool squeak; // 알키 코 누름 여부
    time_t squeakStartTime;  // 코 눌림 상태 시작 시간
    time_t squeakTime;  // 코 눌림 상태동안의 시간

    bool measureTime, isLeave, confirmLeave;
    time_t checkStartTime1, checkEndTime1, checkStartTime2, checkEndTime2;
    bool tired, sleeping;
    time_t sleepReadyTime, sleepCheckTime;
    GLfloat sleepHeight, sleepNum;  // 잠을 잘 때 위 아래로 조금씩 움직인다.
    int breatheType;  // 들숨 / 날숨
    bool breatheSound;  // 중복 재생 방지
    GLfloat delay;
    

    Alcy() {
        dir = m;
        delay = 2;
        breatheType = in_;
    }

    void checkControl() {
        if (!tired) {
            if (!measureTime) {
                checkStartTime1 = time(NULL);
                isLeave = true;  // 컨트롤이 없다고 가정하여 시간 측정을 시작한다.
                measureTime = true;
            }

            if (measureTime && !confirmLeave) {
                checkEndTime1 = time(NULL);
                if (checkEndTime1 - checkStartTime1 >= 1) { // 시간을 1초 간격으로 계속 측정한다.
                    if (isLeave) {
                        checkStartTime2 = time(NULL);
                        confirmLeave = true;  // isLeave에 변동이 없다면 컨트롤이 없음을 확정한다.
                    }
                    else
                        measureTime = false;  // 컨트롤이 존재함을 확인하고 다시 처음 단계로 되돌아간다.
                }
            }

            if (confirmLeave) {  // 컨트롤이 없음이 확정된 상태에서 컨트롤이 감지되면 다시 처음 단계로 되돌아간다.
                if (!isLeave) {
                    measureTime = false;
                    confirmLeave = false;
                }
                else {  //  여전히 계속해서 컨트롤이 없다면 알키는 졸기 시작한다.
                    checkEndTime2 = time(NULL);
                    if (checkEndTime2 - checkStartTime2 >= 9) {
                        sleepReadyTime = time(NULL);
                        tired = true;
                    }
                }
            }
        }
    }

    void updateAlcySleep() {
        if (tired && !sleeping) {  // 졸기 시작하면 마우스로 알키를 직접 클릭하지 않는 이상 꺠지 않는다.
            sleepCheckTime = time(NULL);
            if (lButtonDown) {  // 조는 도중 알키를 클릭할 경우 다시 상태가 초기화 된다.
                measureTime = false;
                confirmLeave = false;
                tired = false;
            }
            if (sleepCheckTime - sleepReadyTime >= 4)  // 졸기 시작하고 5초 뒤 알키는 잠을 자기 시작한다.
                sleeping = true;
        }

        if (tired && sleeping) {
            sleepNum += fs / 4;
            
            sleepHeight = sin(sleepNum) / 80;  // 자는 동안에는 머리가 조금씩 위 아래로 움직인다.
            tailRot = sleepHeight * 100;

            cout << sin(sleepNum) << endl;

            if (sin(sleepNum) > 0.99 && breatheType == in_) {
                breatheType = out_;
                breatheSound = true;
            }
            if (sin(sleepNum) < -0.99 && breatheType == out_) {
                breatheType = in_;
                breatheSound = true;
            }
            
            if (lButtonDown) {  // 자는 도중 알키를 클릭할 경우 다시 상태가 초기화 된다.
                sleepHeight = 0;
                sleepNum = 0;
                tailRot = 0;
                measureTime = false;
                confirmLeave = false;
                tired = false;
                sleeping = false;
                breatheSound = false;
                delay = 2;
                channelBreathe->stop();
            }

            if (breatheSound) {  // 들숨, 날숨 소리가 다르다.
                delay -= fs;

                if (delay < 0) {
                    channelBreathe->stop();
                    if (breatheType == in_)
                        ssystem->playSound(breatheIn, 0, false, &channelBreathe);
                    else if (breatheType == out_)
                        ssystem->playSound(breatheOut, 0, false, &channelBreathe);
                    breatheSound = false;
                    delay = 1;
                }
            }
        }
    }

    void playTiltSound() {  // 머리 기울이는 소리 재생
        if (!tiltSoundPlayed) {
            channelTilt->stop();
            ssystem->playSound(tilt, 0, false, &channelTilt);
            tiltSoundPlayed = true;
        }
    }

    void updateAlcyBlink() {  // 알키 눈 깜빡임 업데이트
        random_device rd;  mt19937 gen(rd());
        uniform_real_distribution <GLfloat> dis(0, 3);

        if (!blinkEnable) {
            endTime = time(NULL);  // 시간을 실시간으로 측정하여 아래의 조건에 맞는지 검사한다
            if (GLfloat(endTime - startTime) > blinkInterval)  // 지정된 간격보다 시간이 지나면 눈 깜빡임이 활성화 된다.
                blinkEnable = true;
        }

        else {
            keepTimer += fs;  // 아주 짧은 시간 동안 눈 감은 상태를 유지하다가 다시 눈을 뜬다.
            if (keepTimer > 1.5) {
                startTime = time(NULL);
                keepTimer = 0;
                blinkInterval = dis(gen);  // 눈 깜빡이는 간격을 랜덤으로 설정한다
                blinkEnable = false;
            }
        }
    }

    void moveAlcyHead() {  // 바라보는 방향 전환 시 알키 머리 움직임
        if (!cam.camR && !cam.camL && cam.camRot == 0 && !squeak && !tired && !sleeping) {
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
        else {  // 아무 조작 없을 때
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

    void updateAlcyTouch() {  // 알키 머리 쓰다듬기
        if (mouseClickEnable && ui.handEnable && touchEnable) {  // 손 커서인 상태로 머리를 쓰다듬을 수 있다
            ui.handX = sin(ui.handNum) * 0.2;  // 쓰다듬는 중에는 손 커서가 좌우로 부드럽게 움직인다.
            ui.handNum += fs / 4;

            tailNum += fs / 10;  // 꼬리는 느린 속도로 별도로 움직인다.
            tailRot = sin(tailNum) * 10;
            headRot = -ui.handX * 25;  // 손의 움직임에 따라 머리도 같이 움직인다.
            bodyRot = -headRot / 4;  // 몸통도 같이 움직인다.
        }
    }

    void tiltAlcyHead() {
        if (!squeak && !tired && !sleeping) {  // 코를 누르는 상태가 아닐 때 머리 각도를 업데이트 한다.
            if (cam.camRot < -9.9) {  // 카메라가 완전히 기울어진 후  알키가 머리를 기울인다
                headTiltL = false;
                headTiltR = true;
                playTiltSound();
            }

            if (cam.camRot > 9.9) {
                headTiltL = true;
                headTiltR = false;
                playTiltSound();
            }

            if (headTiltR) {  // 머리를 오른쪽으로 기울일경우
                headRot -= fs * 2;
                tailRot -= fs * 2;
                if (headRot < -10) {
                    headRot = -10;
                    tailRot = -10;
                }
            }

            if (headTiltL) { // 머리를 왼쪽으로 기울일경우
                headRot += fs * 2;
                tailRot += fs * 2;
                if (headRot > 10) {
                    headRot = 10;
                    tailRot = 10;
                }
            }
        }

        if (cam.camRot == 0 && !cam.camR && !cam.camL && !touchEnable) {  // 카메라가 처음으로 되돌아가면 머리를 다시 가운데로 세운다.
            headTiltR = false;
            headTiltL = false;

            if (headRot > 0) {
                playTiltSound();
                headRot -= fs * 2;
                tailRot -= fs * 2;
                if (headRot < 0) {
                    headRot = 0;
                    tailRot = 0;
                    tiltSoundPlayed = false;
                }
            }
            if (headRot < 0) {
                playTiltSound();
                headRot += fs * 2;
                tailRot += fs * 2;
                if (headRot > 0) {
                    headRot = 0;
                    tailRot = 0;
                    tiltSoundPlayed = false;
                }
            }
        }
    }

    void squeakAlcyNose() {  // 코 누르기
        if (squeak) {  // 일정시간동안 알키는 자기 코를 바라본다
            squeakTime = time(NULL);
            if (squeakTime - squeakStartTime > 1) {  // 1초 후 해제
                squeakStartTime = time(NULL);
                squeak = false;
            }
        }
    }

    void bindVertex(int idx) {  // 변환 전달 
        glBindVertexArray(VAO_ALCY[idx]);  // 각 모델마다 지정된 VAO만 사용
    }

    void setTransform(int idx) {  // 변환 세팅
        using namespace glm;

        switch (idx) {  // 변환 추가
        case tail_:
            translateMatrix = translate(translateMatrix, vec3(-0.2 * ratio, -0.75, -0.00003));
            translateMatrix = rotate(translateMatrix, radians(tailRot), vec3(0.0, 0.0, 1.0));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 0.8, 0.0);
            break;

        case body_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.75 + sleepHeight / 2, -0.00002));
            translateMatrix = rotate(translateMatrix, radians(bodyRot), vec3(0.0, 0.0, 1.0));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 0.8, 0.0);
            break;

        case hair_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.75 - headRot / 200 + sleepHeight, -0.00001));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 0.8, 0.0);
            break;

        case head_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            translateMatrix = translate(translateMatrix, vec3((headPos - headRot / 300) * ratio, 0.22 + sleepHeight, 0.0));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 0.8, 0.0);
            break;

        case eye_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !squeak && !tired && !sleeping)
                translateMatrix = translate(translateMatrix,
                    vec3(((headPos - headRot / 300) - (cam.camX / 4)) * ratio, 0.22 - (cam.camY / 4), 0.00001));
            else
                translateMatrix = translate(translateMatrix,
                    vec3((headPos - headRot / 300) * ratio, 0.22, 0.00001));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 0.9, 0.0);
            break;

        case dot_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            if (cam.camRot == 0 && !cam.camR && !cam.camL)
                translateMatrix = translate(translateMatrix,
                    vec3(((headPos - headRot / 300) - (cam.camX / 2.5)) * ratio, 0.22 - (cam.camY / 2), 0.00003));
            else
                translateMatrix = translate(translateMatrix,vec3((headPos - headRot / 300) * ratio, 0.22, 0.00003));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 1.0, 0.0);
            break;

        case brow_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !touchEnable && !squeak && !tired && !sleeping)
                translateMatrix = translate(translateMatrix,vec3((headPos - headRot / 300) * ratio, 0.23 - (cam.camY / 4), 0.00003));
            else
                translateMatrix = translate(translateMatrix,vec3((headPos - headRot / 300) * ratio, 0.22 + sleepHeight, 0.00003));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 0.8, 0.0);
            break;

        case blink_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            translateMatrix = translate(translateMatrix, vec3((headPos - headRot / 300) * ratio, 0.22 + sleepHeight, 0.00004));
            selectedColor = vec3(0.0, 1.0, 0.0);
            threshold = vec3(0.0, 0.8, 0.0);
            break;
        }

        transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
    }

    void modelOutput(int idx) {  // 모델 출력 
        switch (idx) {
        case tail_:
            glBindTexture(GL_TEXTURE_2D, alcyTail);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case body_:
            glBindTexture(GL_TEXTURE_2D, alcyBody);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case hair_:
            glBindTexture(GL_TEXTURE_2D, alcyHair);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case head_:  // 카메라가 초기 상태일 때 이미지를 업데이트 한다.
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !squeak && !tired && !sleeping) {
                switch (dir) {
                case l: glBindTexture(GL_TEXTURE_2D, alcyHead[1]);  // head left
                    break;
                case r: glBindTexture(GL_TEXTURE_2D, alcyHead[2]);  // head right
                    break;
                case m: glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head middle
                    break;
                }
            }
            else   // 카메라 회전 시 앞을 보도록 함
                glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head middle
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case eye_:  // 카메라가 초기 상태일 때 이미지를 업데이트 한다.
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !squeak && !tired && !sleeping) {
                    switch (dir) {
                    case l: glBindTexture(GL_TEXTURE_2D, eye[1]);  // eye left
                        break;
                    case r: glBindTexture(GL_TEXTURE_2D, eye[2]);  // eye right
                        break;
                    case m: glBindTexture(GL_TEXTURE_2D, eye[0]);  // eye middle
                        break;
                    }
            }

            else { // 카메라 회전 시 앞을 보도록 하고, 상태에 맞는 이미지로 고정되어 출력된다.
                if (squeak)
                    glBindTexture(GL_TEXTURE_2D, eye[3]);  // eye squeak
                else if (tired)
                    glBindTexture(GL_TEXTURE_2D, eye[4]);  // eye tired
                else
                    glBindTexture(GL_TEXTURE_2D, eye[0]);  // eye middle
            }

            if (!blinkEnable && !touchEnable && !sleeping)  // 눈을 깜빡이지 않을 때, 쓰다듬지 않을 때 출력한다.
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case dot_:  // 카메라가 초기 상태일 때만 이미지를 업데이트 한다.
            if (cam.camRot == 0 && !cam.camR && !cam.camL) {
                switch (dir) {
                case l: glBindTexture(GL_TEXTURE_2D, dot[1]);  // eye left
                    break;
                case r: glBindTexture(GL_TEXTURE_2D, dot[2]);  // eye right
                    break;
                case m: glBindTexture(GL_TEXTURE_2D, dot[0]);  // eye middle
                    break;
                }
            }
            else  // 카메라 회전 시 앞을 보도록 함
                glBindTexture(GL_TEXTURE_2D, dot[0]);  // eye middle

            if (!blinkEnable && !touchEnable && !squeak && !tired )  // 눈을 깜빡이지 않을 때, 쓰다듬지 않을 때, 코를 누르지 않을 때 출력한다.
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case brow_:  // 카메라가 초기 상태이면서 코를 누르지 않을 때만 이미지를 업데이트 한다.
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !squeak && !tired && !sleeping) {
                switch (dir) {
                case l: glBindTexture(GL_TEXTURE_2D, brow[1]);  // brow left
                    break;
                case r: glBindTexture(GL_TEXTURE_2D, brow[2]);  // brow right
                    break;
                case m: glBindTexture(GL_TEXTURE_2D, brow[0]);  // brow middle
                    break;
                }
            }
            else  // 카메라 회전 시 앞을 보도록 함
                glBindTexture(GL_TEXTURE_2D, brow[0]);  // brow middle
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case blink_:  // 카메라가 초기 상태이면서 코를 누르지 않을 때만 이미지를 업데이트 한다.
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !squeak && !tired && !sleeping) {
                switch (dir) {
                case l: glBindTexture(GL_TEXTURE_2D, blink[1]);  // blink left
                    break;
                case r: glBindTexture(GL_TEXTURE_2D, blink[2]);  // blink right
                    break;
                case m: glBindTexture(GL_TEXTURE_2D, blink[0]);  // blink middle
                    break;
                }
            }
            else
                glBindTexture(GL_TEXTURE_2D, blink[0]);  // blink middle

            if (blinkEnable || touchEnable || sleeping)  // 셋 중 하나가 true일 때만 출력
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;
        }
    }
};

extern Alcy alcy;

#endif