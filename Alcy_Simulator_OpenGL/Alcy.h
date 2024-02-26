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

enum dir {  // ��Ű �ٶ󺸴� ����
    l, m, r
};

enum name {  // �� �̹��� ��� ���̽� ����
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

	int dir;  // ��Ű �ٶ󺸴� ����, �ʱⰪ m

	bool blinkEnable; // �� ������ ����, true�� �� ������ Ȱ��ȭ
	time_t startTime, endTime, blinkTime;  // �� ������ ���� Ÿ�̸�
	GLfloat blinkInterval;  // �� ������ ����, �⺻ 3��
	GLfloat keepTimer;  // ���� ���� ���¸� ���� ª�� �����Ѵ�
	bool touchEnable;  // ��Ű �Ӹ� ���ٵ�� ����

	//��Ű �Ӹ�, �� ���� ������
	GLfloat headPos;  // ���� ��ȯ �� ��ȯ�� �������� ������
	GLfloat headRot;  // ���ٵ�� �� �Ӹ� ȸ�� ����
	GLfloat tailRot;  // ���ٵ�� �� ���� ȸ�� ����
	GLfloat bodyRot;  // ���ٵ�� �� ���� ȸ�� ����
	GLfloat tailNum; // ���� ȸ���� ���Ǵ� ��ġ
    GLfloat headNum;
	bool headTiltR, headTiltL;  // ī�޶� ��￴�� �� ��Ű �Ӹ� ����̱� ����
    bool tiltSoundPlayed;  // ��Ű �Ӹ� ����̴� �Ҹ� ��� ����, �ߺ� ��� ����

    bool squeak; // ��Ű �� ���� ����
    GLfloat squeakTime;  // �� ���� ���µ����� �ð�

    bool measureTime, isLeave, confirmLeave;
    time_t checkStartTime1, checkEndTime1, checkStartTime2, checkEndTime2;
    bool tired, sleeping;
    time_t sleepReadyTime, sleepCheckTime;
    GLfloat sleepHeight, sleepNum;  // ���� �� �� �� �Ʒ��� ���ݾ� �����δ�.
    int breatheType;  // ��� / ����
    bool breatheSound;  // �ߺ� ��� ����
    GLfloat delay;  // ������ �Ҹ� ������
    
    GLfloat beatX, beatY;  // ���� ���� ȿ��
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
                isLeave = true;  // ��Ʈ���� ���ٰ� �����Ͽ� �ð� ������ �����Ѵ�.
                measureTime = true;
            }

            if (measureTime && !confirmLeave) {
                checkEndTime1 = time(NULL);
                if (checkEndTime1 - checkStartTime1 >= 1) { // �ð��� 1�� �������� ��� �����Ѵ�.
                    if (isLeave) {
                        checkStartTime2 = time(NULL);
                        confirmLeave = true;  // isLeave�� ������ ���ٸ� ��Ʈ���� ������ Ȯ���Ѵ�.
                    }
                    else
                        measureTime = false;  // ��Ʈ���� �������� Ȯ���ϰ� �ٽ� ó�� �ܰ�� �ǵ��ư���.
                }
            }

            if (confirmLeave) {  // ��Ʈ���� ������ Ȯ���� ���¿��� ��Ʈ���� �����Ǹ� �ٽ� ó�� �ܰ�� �ǵ��ư���.
                if (!isLeave) {
                    measureTime = false;
                    confirmLeave = false;
                }
                else {  //  ������ ����ؼ� ��Ʈ���� ���ٸ� ��Ű�� ���� �����Ѵ�.
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
        if (tired && !sleeping) {  // ���� �����ϸ� ���콺�� ��Ű�� ���� Ŭ������ �ʴ� �̻� ���� �ʴ´�.
            sleepCheckTime = time(NULL);
            if (lButtonDown) {  // ���� ���� ��Ű�� Ŭ���� ��� �ٽ� ���°� �ʱ�ȭ �ȴ�.
                measureTime = false;
                confirmLeave = false;
                tired = false;
            }
            if (sleepCheckTime - sleepReadyTime >= 4)  // ���� �����ϰ� 5�� �� ��Ű�� ���� �ڱ� �����Ѵ�.
                sleeping = true;
        }

        if (tired && sleeping) {
            sleepNum += fs / 4;
            
            sleepHeight = sin(sleepNum) / 80;  // �ڴ� ���ȿ��� �Ӹ��� ���ݾ� �� �Ʒ��� �����δ�.
            tailRot = sleepHeight * 100;

            if (sin(sleepNum) > 0.99 && breatheType == in_) {
                breatheType = out_;
                breatheSound = true;
            }
            if (sin(sleepNum) < -0.99 && breatheType == out_) {
                breatheType = in_;
                breatheSound = true;
            }
            
            if (lButtonDown) {  // �ڴ� ���� ��Ű�� Ŭ���� ��� �ٽ� ���°� �ʱ�ȭ �ȴ�.
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

            if (breatheSound) {  // ���, ���� �Ҹ��� �ٸ���.
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

    void updateAlcyBlink() {  // ��Ű �� ������ ������Ʈ
        random_device rd;  mt19937 gen(rd());
        uniform_real_distribution <GLfloat> dis(0, 3);

        if (!blinkEnable) {
            endTime = time(NULL);  // �ð��� �ǽð����� �����Ͽ� �Ʒ��� ���ǿ� �´��� �˻��Ѵ�
            if (GLfloat(endTime - startTime) > blinkInterval)  // ������ ���ݺ��� �ð��� ������ �� �������� Ȱ��ȭ �ȴ�.
                blinkEnable = true;
        }

        else {
            keepTimer += fs;  // ���� ª�� �ð� ���� �� ���� ���¸� �����ϴٰ� �ٽ� ���� ���.
            if (keepTimer > 1.5) {
                startTime = time(NULL);
                keepTimer = 0;
                blinkInterval = dis(gen);  // �� �����̴� ������ �������� �����Ѵ�
                blinkEnable = false;
            }
        }
    }

    void tiltAlcyHead() {
        if (!squeak && !tired && !sleeping) {  // �ڸ� ������ ���°� �ƴ� �� �Ӹ� ������ ������Ʈ �Ѵ�.
            if (cam.camRot < -9.9) {  // ī�޶� ������ ������ ��  ��Ű�� �Ӹ��� ����δ�
                headTiltL = false;
                headTiltR = true;
                playTiltSound();
            }

            if (cam.camRot > 9.9) {
                headTiltL = true;
                headTiltR = false;
                playTiltSound();
            }

            if (headTiltR) {  // �Ӹ��� ���������� ����ϰ��
                headRot -= fs * 2;
                tailRot -= fs * 2;
                if (headRot < -10) {
                    headRot = -10;
                    tailRot = -10;
                }
            }

            if (headTiltL) { // �Ӹ��� �������� ����ϰ��
                headRot += fs * 2;
                tailRot += fs * 2;
                if (headRot > 10) {
                    headRot = 10;
                    tailRot = 10;
                }
            }
        }

        if (cam.camRot == 0 && !cam.camR && !cam.camL && !touchEnable) {  // ī�޶� ó������ �ǵ��ư��� �Ӹ��� �ٽ� ����� �����.
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

    void playTiltSound() {  // �Ӹ� ����̴� �Ҹ� ���
        if (!tiltSoundPlayed) {
            channelTilt->stop();
            ssystem->playSound(tilt, 0, false, &channelTilt);
            tiltSoundPlayed = true;
        }
    }

    void moveAlcyHead() {  // �ٶ󺸴� ���� ��ȯ �� ��Ű �Ӹ� ������
        if (!cam.camR && !cam.camL && cam.camRot == 0 && !squeak && !tired && !sleeping) {
            switch (dir) {
            case l:  // ���� �ٶ� ��
                headPos -= 0.04 * fs;
                if (headPos < -0.08)
                    headPos = -0.08;
                break;

            case r:  // ���� �ٶ� ��
                headPos += 0.04 * fs;
                if (headPos > 0.08)
                    headPos = 0.08;
                break;

            default:  // ��� �ٶ� ��
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
        else {  // �ƹ� ���� ���� ��
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

    void updateAlcyTouch() {  // ��Ű �Ӹ� ���ٵ��
        if (mouseClickEnable && ui.handEnable && touchEnable) {  // �� Ŀ���� ���·� �Ӹ��� ���ٵ��� �� �ִ�
            ui.handX = sin(ui.handNum) * 0.4;  // ���ٵ�� �߿��� �� Ŀ���� �¿�� �ε巴�� �����δ�.
            ui.handNum += fs / 4;

            tailNum += fs / 10;  // ������ ���� �ӵ��� ������ �����δ�.
            tailRot = sin(tailNum) * 10;
            headRot = -ui.handX * 17;  // ���� �����ӿ� ���� �Ӹ��� ���� �����δ�.
            bodyRot = -headRot / 4;  // ���뵵 ���� �����δ�.
        }
    }

    void squeakAlcyNose() {  // �� ������
        if (squeak) {  // �����ð����� ��Ű�� �ڱ� �ڸ� �ٶ󺻴�
            squeakTime += fs;
            if (squeakTime > 15) {  // 1.5�� �� ����
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

        switch (randomSound) {  // 3���� �� �ϳ��� ����Ѵ�.
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

                if (beatDir % 2 == 0) {  // ���� ������ ������ ���� �Ӹ��� ����.
                    beatX = beatVal / 2;
                    beatY = -1 * beatVal;
                }
                else if (beatDir % 2 == 1) {
                    beatX = -1 * beatVal / 2;
                    beatY = -1 * beatVal;
                }
            }

            else {
                if (beatDir % 2 == 0) {  // ���� ������ ������ ���� �Ӹ��� ����.
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

    void setBuffer() {  // ��Ű ���� �ʱ�ȭ
        glGenVertexArrays(1, &VAO_ALCY);
        glBindVertexArray(VAO_ALCY);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        vertexInput();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // ��ġ �Ӽ�
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // �ؽ�ó ��ǥ �Ӽ� 
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

    void modelOutput(int idx) {  // �� ��� 
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

        case head_:  // ī�޶� �ʱ� ������ �� �̹����� ������Ʈ �Ѵ�.
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
            else {  // ī�޶� ȸ�� �� ���� ������ ��
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

        case eye_:  // ī�޶� �ʱ� ������ �� �̹����� ������Ʈ �Ѵ�.
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

            else { // ī�޶� ȸ�� �� ���� ������ �ϰ�, ���¿� �´� �̹����� �����Ǿ� ��µȴ�.
                if (squeak)
                    glBindTexture(GL_TEXTURE_2D, eye[3]);  // eye squeak
                else if (tired)
                    glBindTexture(GL_TEXTURE_2D, eye[4]);  // eye tired
                else
                    glBindTexture(GL_TEXTURE_2D, eye[0]);  // eye middle
            }
            if (!blinkEnable && !touchEnable && !sleeping)  // ���� �������� ���� ��, ���ٵ��� ���� �� ����Ѵ�.
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case dot_:  // ī�޶� �ʱ� ������ ���� �̹����� ������Ʈ �Ѵ�.
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
            else  // ī�޶� ȸ�� �� ���� ������ ��
                glBindTexture(GL_TEXTURE_2D, dot[0]);  // eye middle

            if (!blinkEnable && !touchEnable && !squeak && !tired)  // ���� �������� ���� ��, ���ٵ��� ���� ��, �ڸ� ������ ���� �� ����Ѵ�.
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case brow_:  // ī�޶� �ʱ� �����̸鼭 �ڸ� ������ ���� ���� �̹����� ������Ʈ �Ѵ�.
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
            else  // ī�޶� ȸ�� �� ���� ������ ��
                glBindTexture(GL_TEXTURE_2D, brow[0]);  // brow middle
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case blink_:  // ī�޶� �ʱ� �����̸鼭 �ڸ� ������ ���� ���� �̹����� ������Ʈ �Ѵ�.
            if (playFunc) break;

            if (blinkEnable || touchEnable || sleeping) { // �� �� �ϳ��� true�� ���� ���
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

    void setObject(int idx) {  // ��ȯ ����
        using namespace glm;

        initTransform();

        switch (idx) {  // ��ȯ �߰�
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

        transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ
        transmit();

        glBindVertexArray(VAO_ALCY);  // �� �𵨸��� ������ VAO�� ���
        modelOutput(idx);
    }
};

extern Alcy alcy;

#endif