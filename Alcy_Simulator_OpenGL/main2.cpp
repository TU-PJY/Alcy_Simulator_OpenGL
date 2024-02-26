#include "config.h"  // �߿� ��� ����
#include "transform.h"  // ��ȯ
#include "globalVar.h"
#include "buffer.h"
#include "texture.h"

#include "Background.h"
#include "Alcy.h"  
#include "UI.h"
#include "ZZZ.h"
#include "Icon.h"
#include "White.h"
#include "Turntable.h"
#include "Speaker.h"
#include "Guitar.h"
#include "Arm.h"
#include "Light.h"
#include "main2.h"

// �̹��� ��� cpp

void initObject() {
	// ���� �ʱ�ȭ
	background.setBuffer();  // ��� �ʱ�ȭ
	alcy.setBuffer();  // ��Ű �ʱ�ȭ
	ui.setBuffer();  // ui �ʱ�ȭ
	turntable.setBuffer();  // �����̺� �ʱ�ȭ
	speaker.setBuffer(); // ����Ŀ �ʱ�ȭ
	guitar.setBuffer(); // ��Ÿ �ʱ�ȭ
	arm.setBuffer(); // �� �ʱ�ȭ
	light.setBuffer();  // �� �ʱ�ȭ
	white.setBuffer();  // �� ��� �ʱ�ȭ

	for (int i = 0; i < ICON_PART; i++)  // �޴� ������ �ʱ�ȭ
		icon[i].setBuffer();
	for (int i = 0; i < 3; i++)  // zzz������Ʈ �ʱ�ȭ
		zzz[i].setBuffer();


	// �ؽ�ó ����
	background.setTexture();  // ���
	alcy.setTexture();  // ��Ű
	ui.setTexture();  // UI
	turntable.setTexture();
	speaker.setTexture();
	guitar.setTexture(); 
	arm.setTexture();
	white.setTexture();
	light.setTexture();

	for (int i = 0; i < ICON_PART; i++)
		icon[i].setTexture(i);
	for (int i = 0; i < 3; i++) {// zzz������Ʈ
		zzz[i].setTexture();
		zzz[i].setDelay(i);
	}
}


//  �Ʒ��� �� ���� �� ���̾ �׷���
void objectOutput() {
	// ���
	background.setObject();

	// ����Ŀ
	if (playFunc && (funcNumber == 0 || funcNumber == 1)) 
		speaker.setObject();

	// Alcy
	for (int i = 0; i < ALCY_PART; i++)
		alcy.setObject(i);

	if (playFunc) {
		// �����̺�
		if (funcNumber == 0 || funcNumber == 1) 
			turntable.setObject();

		// ��Ÿ, ��
		else if (funcNumber == 2 || funcNumber == 3) {
			guitar.setObject();
			arm.setObject();

			if (funcNumber == 3)  // ��
				light.setObject();
		}
	}

	// ZZZ �̹���
	for (int i = 0; i < 3; i++)
		if (zzz[i].zzzTransparent > 0.0)
			zzz[i].setObject();

	// UI
	for (int i = 0; i < UI_PART; i++)
		ui.setObject(i);

	// �޴� ������
	for (int i = 0; i < ICON_PART; i++) 
		if (icon[i].iconTransparent > 0.0) 
			icon[i].setObject(i);

	//�� ���
	if (whiteTransparent > 0.0) 
		white.setObject();
}