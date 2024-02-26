#ifndef ALCY_H
#define ALCY_H
#include "config.h"
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
    GLuint VAO_ALCY;

    unsigned int alcyTail[2], alcyBody[3], alcyHair[2], alcyHead[9];
    unsigned int eye[5], dot[3], eyeClose[3], brow[3], blink[3];

    int channel;
    int alcyW = 1500, alcyH = 1500;

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
    GLfloat headNum;
	bool headTiltR, headTiltL;  // 카메라 기울였을 때 알키 머리 기울이기 여부
    bool tiltSoundPlayed;  // 알키 머리 기울이는 소리 재생 여부, 중복 재생 방지

    bool squeak; // 알키 코 누름 여부
    GLfloat squeakTime;  // 코 눌림 상태동안의 시간

    bool measureTime, isLeave, confirmLeave;
    time_t checkStartTime1, checkEndTime1, checkStartTime2, checkEndTime2;
    bool tired, sleeping;
    time_t sleepReadyTime, sleepCheckTime;
    GLfloat sleepHeight, sleepNum;  // 잠을 잘 때 위 아래로 조금씩 움직인다.
    int breatheType;  // 들숨 / 날숨
    bool breatheSound;  // 중복 재생 방지
    GLfloat delay;  // 숨쉬는 소리 딜레이
    
    GLfloat beatX, beatY;  // 음악 박자 효과
    int beatDir;
    bool setOn;
    GLfloat Imageidx;


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

    void playTiltSound() {  // 머리 기울이는 소리 재생
        if (!tiltSoundPlayed) {
            channelTilt->stop();
            ssystem->playSound(tilt, 0, false, &channelTilt);
            tiltSoundPlayed = true;
        }
    }

    void moveAlcyHead() {  // 바라보는 방향 전환 시 알키 머리 움직임
        if (!cam.camR && !cam.camL && cam.camRot == 0 && !squeak && !tired && !sleeping) {
            switch (dir) {
            case l:  // 좌측 바라볼 시
                headPos -= 0.04 * fs;
                if (headPos < -0.08)
                    headPos = -0.08;
                break;

            case r:  // 우측 바라볼 시
                headPos += 0.04 * fs;
                if (headPos > 0.08)
                    headPos = 0.08;
                break;

            default:  // 가운데 바라볼 시
                if (headPos < 0) {
                    headPos += 0.04 * fs;
                    if (headPos > 0)
                        headPos = 0;
                }
                if (headPos > 0) {
                    headPos -= 0.04 * fs;
                    if (headPos < 0)
                        headPos = 0;
                }
                break;
            }
        }
        else {  // 아무 조작 없을 때
            if (headPos < 0) {
                headPos += 0.04 * fs;
                if (headPos > 0)
                    headPos = 0;
            }
            if (headPos > 0) {
                headPos -= 0.04 * fs;
                if (headPos < 0)
                    headPos = 0;
            }
        }
    }

    void updateAlcyTouch() {  // 알키 머리 쓰다듬기
        if (mouseClickEnable && ui.handEnable && touchEnable) {  // 손 커서인 상태로 머리를 쓰다듬을 수 있다
            ui.handX = sin(ui.handNum) * 0.4;  // 쓰다듬는 중에는 손 커서가 좌우로 부드럽게 움직인다.
            ui.handNum += fs / 4;

            tailNum += fs / 10;  // 꼬리는 느린 속도로 별도로 움직인다.
            tailRot = sin(tailNum) * 10;
            headRot = -ui.handX * 17;  // 손의 움직임에 따라 머리도 같이 움직인다.
            bodyRot = -headRot / 4;  // 몸통도 같이 움직인다.
        }
    }

    void squeakAlcyNose() {  // 코 누르기
        if (squeak) {  // 일정시간동안 알키는 자기 코를 바라본다
            squeakTime += fs;
            if (squeakTime > 15) {  // 1.5초 후 해제
                squeak = false;
                squeakTime = 0;
            }
        }
    }

    void playAlcySqueakSound() {
        random_device rd;  mt19937 gen(rd());
        uniform_int_distribution <int> dis(1, 3);
        int randomSound = dis(gen);

        channelSqueak->stop();

        switch (randomSound) {  // 3가지 중 하나를 재생한다.
        case 1: ssystem->playSound(squeak1, 0, false, &channelSqueak);
            break;
        case 2: ssystem->playSound(squeak2, 0, false, &channelSqueak);
            break;
        case 3: ssystem->playSound(squeak3, 0, false, &channelSqueak);
            break;
        }

        squeak = true;
    }

    void updateAlcyBeat() {
        if (funcNumber == 0) {
            if (beatDelay >= interval) {
                beatDir += 1;

                if (beatDir % 2 == 0) {  // 왼쪽 오른쪽 번갈아 가며 머리를 흔든다.
                    beatX = beatVal / 2;
                    beatY = -1 * beatVal;
                }
                else if (beatDir % 2 == 1) {
                    beatX = -1 * beatVal / 2;
                    beatY = -1 * beatVal;
                }
            }

            else {
                if (beatDir % 2 == 0) {  // 왼쪽 오른쪽 번갈아 가며 머리를 흔든다.
                    beatX = beatVal / 2;
                    beatY = -1 * beatVal;
                }
                else if (beatDir % 2 == 1) {
                    beatX = -1 * beatVal / 2;
                    beatY = -1 * beatVal;
                }
            }
        }

        if (funcNumber == 1) {
            if (beatDelay >= interval)
                setOn = true;

            if (setOn) {
                beatY -= fs / 5;
                if (beatY < -0.15) {
                    beatY = -0.15;
                    setOn = false;
                }
            }

            else if (!setOn) {
                beatY += fs / 30;
                if (beatY > 0)
                    beatY = 0;
            }
        }
    }

    void updateImageIndex() {
        Imageidx += fs;
        if (Imageidx > 3.99999)
            Imageidx = 0;
    }

    void updateAlcyGuitarPlay() {
        headRot = 10 + sin(headNum) * 3;
        tailRot = sin(tailNum) * 2;
        headNum += fs / 5;
        tailNum += fs / 5;
    }

    void setBuffer() {  // 알키 버퍼 초기화
        glGenVertexArrays(1, &VAO_ALCY);
        glBindVertexArray(VAO_ALCY);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        vertexInput();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
        glEnableVertexAttribArray(2);
    }

    void setTexture() {
        // tail
        glGenTextures(1, &alcyTail[0]);
        glBindTexture(GL_TEXTURE_2D, alcyTail[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//tail.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // tail guitar 1
        glGenTextures(1, &alcyTail[1]);
        glBindTexture(GL_TEXTURE_2D, alcyTail[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//tail_guitar1.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy body
        glGenTextures(1, &alcyBody[0]);
        glBindTexture(GL_TEXTURE_2D, alcyBody[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//body.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy body guitar
        glGenTextures(1, &alcyBody[1]);
        glBindTexture(GL_TEXTURE_2D, alcyBody[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//body_guitar1.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy body guitar2
        glGenTextures(1, &alcyBody[2]);
        glBindTexture(GL_TEXTURE_2D, alcyBody[2]);
        parameteri();
        texture_data = stbi_load("res//alcy//body_guitar2.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy hair
        glGenTextures(1, &alcyHair[0]);
        glBindTexture(GL_TEXTURE_2D, alcyHair[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//hair.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy hair guitar 1
        glGenTextures(1, &alcyHair[1]);
        glBindTexture(GL_TEXTURE_2D, alcyHair[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//hair_guitar1.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy head middle
        glGenTextures(1, &alcyHead[0]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_middle.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy head left
        glGenTextures(1, &alcyHead[1]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_left.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // alcy head right
        glGenTextures(1, &alcyHead[2]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[2]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_right.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        //alcy head house
        glGenTextures(1, &alcyHead[3]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[3]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_house.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        //alcy head glitch //////////////////
        glGenTextures(1, &alcyHead[4]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[4]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_glitch_1.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        glGenTextures(1, &alcyHead[5]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[5]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_glitch_2.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        glGenTextures(1, &alcyHead[6]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[6]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_glitch_3.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        glGenTextures(1, &alcyHead[7]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[7]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_glitch_4.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        ///////////////

        // alcy head guitar1
        glGenTextures(1, &alcyHead[8]);
        glBindTexture(GL_TEXTURE_2D, alcyHead[8]);
        parameteri();
        texture_data = stbi_load("res//alcy//head_guitar1.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);


        // eye middle
        glGenTextures(1, &eye[0]);
        glBindTexture(GL_TEXTURE_2D, eye[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//eye_middle.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // eye left
        glGenTextures(1, &eye[1]);
        glBindTexture(GL_TEXTURE_2D, eye[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//eye_left.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // eye right
        glGenTextures(1, &eye[2]);
        glBindTexture(GL_TEXTURE_2D, eye[2]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//eye_right.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // eye squeak
        glGenTextures(1, &eye[3]);
        glBindTexture(GL_TEXTURE_2D, eye[3]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//eye_squeak.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // eye tired
        glGenTextures(1, &eye[4]);
        glBindTexture(GL_TEXTURE_2D, eye[4]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//eye_tired.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // dot middle
        glGenTextures(1, &dot[0]);
        glBindTexture(GL_TEXTURE_2D, dot[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//dot_middle.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // dot left
        glGenTextures(1, &dot[1]);
        glBindTexture(GL_TEXTURE_2D, dot[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//dot_left.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        // dot right
        glGenTextures(1, &dot[2]);
        glBindTexture(GL_TEXTURE_2D, dot[2]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//dot_right.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);



        //brow middle
        glGenTextures(1, &brow[0]);
        glBindTexture(GL_TEXTURE_2D, brow[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//brow_middle.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        //brow left
        glGenTextures(1, &brow[1]);
        glBindTexture(GL_TEXTURE_2D, brow[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//brow_left.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        //brow right
        glGenTextures(1, &brow[2]);
        glBindTexture(GL_TEXTURE_2D, brow[2]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//brow_right.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        //blink middle
        glGenTextures(1, &blink[0]);
        glBindTexture(GL_TEXTURE_2D, blink[0]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//blink_middle.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        //blink left
        glGenTextures(1, &blink[1]);
        glBindTexture(GL_TEXTURE_2D, blink[1]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//blink_left.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        //blink right
        glGenTextures(1, &blink[2]);
        glBindTexture(GL_TEXTURE_2D, blink[2]);
        parameteri();
        texture_data = stbi_load("res//alcy//face//blink_right.png", &alcyW, &alcyH, &channel, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    }

    void modelOutput(int idx) {  // 모델 출력 
        switch (idx) {
        case tail_:
            if (playFunc && funcNumber == 2)
                glBindTexture(GL_TEXTURE_2D, alcyTail[1]);
            else
                glBindTexture(GL_TEXTURE_2D, alcyTail[0]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case body_:
            if (playFunc && funcNumber == 2)
                glBindTexture(GL_TEXTURE_2D, alcyBody[1]);
            else if (playFunc && funcNumber == 3)
                glBindTexture(GL_TEXTURE_2D, alcyBody[2]);
            else
                glBindTexture(GL_TEXTURE_2D, alcyBody[0]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case hair_:
            if (playFunc && funcNumber == 2)
                glBindTexture(GL_TEXTURE_2D, alcyHair[1]);
            else
                glBindTexture(GL_TEXTURE_2D, alcyHair[0]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case head_:  // 카메라가 초기 상태일 때 이미지를 업데이트 한다.
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !squeak && !tired && !sleeping && !playFunc) {
                switch (dir) {
                case l: glBindTexture(GL_TEXTURE_2D, alcyHead[1]);  // head left
                    break;
                case r: glBindTexture(GL_TEXTURE_2D, alcyHead[2]);  // head right
                    break;
                case m: glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head middle
                    break;
                }
            }
            else {  // 카메라 회전 시 앞을 보도록 함
                if (!playFunc) {
                    glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head middle
                }
                else {
                    if (funcNumber == 0)
                        glBindTexture(GL_TEXTURE_2D, alcyHead[3]);  // head house
                    else if (funcNumber == 1)
                        glBindTexture(GL_TEXTURE_2D, alcyHead[(int)Imageidx + 4]);  // head glitch 1 ~ 4 (idx = 4 ~ 7)
                    else if (funcNumber == 2)
                        glBindTexture(GL_TEXTURE_2D, alcyHead[8]);  // head guitar1
                }
            }
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case eye_:  // 카메라가 초기 상태일 때 이미지를 업데이트 한다.
            if (playFunc || blinkEnable || touchEnable) break;

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
            if (playFunc || blinkEnable || touchEnable) break;

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

            if (!blinkEnable && !touchEnable && !squeak && !tired)  // 눈을 깜빡이지 않을 때, 쓰다듬지 않을 때, 코를 누르지 않을 때 출력한다.
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case brow_:  // 카메라가 초기 상태이면서 코를 누르지 않을 때만 이미지를 업데이트 한다.
            if (playFunc) break;

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
            if (playFunc) break;

            if (blinkEnable || touchEnable || sleeping) { // 셋 중 하나가 true일 때만 출력
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
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            break;
        }
    }

    void setObject(int idx) {  // 변환 세팅
        using namespace glm;

        initTransform();

        switch (idx) {  // 변환 추가
        case tail_:
            translateMatrix = translate(translateMatrix, vec3(-0.4, -0.75, -0.0008));
            translateMatrix = rotate(translateMatrix, radians(tailRot), vec3(0.0, 0.0, 1.0));
            break;

        case body_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.75 + sleepHeight / 2, -0.0005));
            translateMatrix = rotate(translateMatrix, radians(bodyRot), vec3(0.0, 0.0, 1.0));
            break;

        case hair_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.75 - headRot / 200 + sleepHeight, -0.0003));
            break;

        case head_:
            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, -0.00001));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            translateMatrix = translate(translateMatrix, vec3(headPos - headRot / 150 + beatX, 0.22 + sleepHeight + beatY, -0.0001));
            break;

        case eye_:
            if (playFunc || blinkEnable) break;

            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !squeak && !tired && !sleeping)
                translateMatrix = translate(translateMatrix,
                    vec3(((headPos - headRot / 150) - (cam.camX / 2.3)), 0.22 - (cam.camY / 2.3), 0.00001));
            else
                translateMatrix = translate(translateMatrix,
                    vec3((headPos - headRot / 150), 0.22, 0.00001));
            break;

        case dot_:
            if (playFunc) break;

            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            if (cam.camRot == 0 && !cam.camR && !cam.camL)
                translateMatrix = translate(translateMatrix,
                    vec3(((headPos - headRot / 150) - (cam.camX / 1.5)), 0.22 - (cam.camY / 1.5), 0.00003));
            else
                translateMatrix = translate(translateMatrix,vec3((headPos - headRot / 150), 0.22, 0.00003));
            break;

        case brow_:
            if (playFunc) break;

            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            if (cam.camRot == 0 && !cam.camR && !cam.camL && !touchEnable && !squeak && !tired && !sleeping)
                translateMatrix = translate(translateMatrix,vec3((headPos - headRot / 150), 0.23 - (cam.camY / 4), 0.00003));
            else
                translateMatrix = translate(translateMatrix,vec3((headPos - headRot / 150), 0.22 + sleepHeight, 0.00003));
            break;

        case blink_:
            if (playFunc) break;

            translateMatrix = translate(translateMatrix, vec3(0.0, -0.1, 0.0));
            translateMatrix = rotate(translateMatrix, radians(headRot), vec3(0.0, 0.0, 1.0));
            translateMatrix = translate(translateMatrix, vec3((headPos - headRot / 150), 0.22 + sleepHeight, 0.00004));
            break;
        }

        transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // 최종 변환
        transmit();

        glBindVertexArray(VAO_ALCY);  // 각 모델마다 지정된 VAO만 사용
        modelOutput(idx);
    }
};

extern Alcy alcy;

#endif