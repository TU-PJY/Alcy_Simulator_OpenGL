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
	bool headTiltR, headTiltL;  // ī�޶� ��￴�� �� ��Ű �Ӹ� ����̱� ����
    bool tiltSoundPlayed;  // ��Ű �Ӹ� ����̴� �Ҹ� ��� ����, �ߺ� ��� ����

    bool squeak; // ��Ű �� ���� ����
    time_t squeakStartTime;  // �� ���� ���� ���� �ð�
    time_t squeakTime;  // �� ���� ���µ����� �ð�

    bool measureTime, isLeave, confirmLeave;
    time_t checkStartTime1, checkEndTime1, checkStartTime2, checkEndTime2;
    bool tired, sleeping;
    time_t sleepReadyTime, sleepCheckTime;
    GLfloat sleepHeight, sleepNum;  // ���� �� �� �� �Ʒ��� ���ݾ� �����δ�.
    int breatheType;  // ��� / ����
    bool breatheSound;  // �ߺ� ��� ����
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

            cout << sin(sleepNum) << endl;

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

    void playTiltSound() {  // �Ӹ� ����̴� �Ҹ� ���
        if (!tiltSoundPlayed) {
            channelTilt->stop();
            ssystem->playSound(tilt, 0, false, &channelTilt);
            tiltSoundPlayed = true;
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

    void moveAlcyHead() {  // �ٶ󺸴� ���� ��ȯ �� ��Ű �Ӹ� ������
        if (!cam.camR && !cam.camL && cam.camRot == 0 && !squeak && !tired && !sleeping) {
            switch (dir) {
            case l:  // ���� �ٶ� ��
                headPos -= 0.03 * fs;
                if (headPos < -0.05)
                    headPos = -0.05;
                break;

            case r:  // ���� �ٶ� ��
                headPos += 0.03 * fs;
                if (headPos > 0.05)
                    headPos = 0.05;
                break;

            default:  // ��� �ٶ� ��
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
        else {  // �ƹ� ���� ���� ��
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

    void updateAlcyTouch() {  // ��Ű �Ӹ� ���ٵ��
        if (mouseClickEnable && ui.handEnable && touchEnable) {  // �� Ŀ���� ���·� �Ӹ��� ���ٵ��� �� �ִ�
            ui.handX = sin(ui.handNum) * 0.2;  // ���ٵ�� �߿��� �� Ŀ���� �¿�� �ε巴�� �����δ�.
            ui.handNum += fs / 4;

            tailNum += fs / 10;  // ������ ���� �ӵ��� ������ �����δ�.
            tailRot = sin(tailNum) * 10;
            headRot = -ui.handX * 25;  // ���� �����ӿ� ���� �Ӹ��� ���� �����δ�.
            bodyRot = -headRot / 4;  // ���뵵 ���� �����δ�.
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

    void squeakAlcyNose() {  // �� ������
        if (squeak) {  // �����ð����� ��Ű�� �ڱ� �ڸ� �ٶ󺻴�
            squeakTime = time(NULL);
            if (squeakTime - squeakStartTime > 1) {  // 1�� �� ����
                squeakStartTime = time(NULL);
                squeak = false;
            }
        }
    }

    void bindVertex(int idx) {  // ��ȯ ���� 
        glBindVertexArray(VAO_ALCY[idx]);  // �� �𵨸��� ������ VAO�� ���
    }

    void setTransform(int idx) {  // ��ȯ ����
        using namespace glm;

        switch (idx) {  // ��ȯ �߰�
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

        transformMatrix = rotateMatrix * translateMatrix * scaleMatrix;  // ���� ��ȯ
    }

    void modelOutput(int idx) {  // �� ��� 
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

        case head_:  // ī�޶� �ʱ� ������ �� �̹����� ������Ʈ �Ѵ�.
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
            else   // ī�޶� ȸ�� �� ���� ������ ��
                glBindTexture(GL_TEXTURE_2D, alcyHead[0]);  // head middle
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case eye_:  // ī�޶� �ʱ� ������ �� �̹����� ������Ʈ �Ѵ�.
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

            if (!blinkEnable && !touchEnable && !squeak && !tired )  // ���� �������� ���� ��, ���ٵ��� ���� ��, �ڸ� ������ ���� �� ����Ѵ�.
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;

        case brow_:  // ī�޶� �ʱ� �����̸鼭 �ڸ� ������ ���� ���� �̹����� ������Ʈ �Ѵ�.
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

            if (blinkEnable || touchEnable || sleeping)  // �� �� �ϳ��� true�� ���� ���
                glDrawArrays(GL_TRIANGLES, 0, 6);
            break;
        }
    }
};

extern Alcy alcy;

#endif