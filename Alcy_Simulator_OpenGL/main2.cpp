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
#include "Mic.h"
#include "Neon.h"
#include "Note.h"
#include "Info.h"
#include "main2.h"

// ������Ʈ ���� �� ���

void initObject() {  // ������Ʈ ����
	// ������Ʈ ���� ����
	// ���� ������Ʈ
	background.setBuffer(); 
	alcy.setBuffer();

	// ���� ������Ʈ
	turntable.setBuffer(); 
	speaker.setBuffer();
	guitar.setBuffer();
	arm.setBuffer();
	light.setBuffer();
	mic.setBuffer();
	neon.setBuffer();
	note.setBuffer();
	for (int i = 0; i < 3; i++)
		zzz[i].setBuffer();

	// UI ������Ʈ
	ui.setBuffer();
	info.setBuffer();
	white.setBuffer();
	for (int i = 0; i < ICON_PART; i++) 
		icon[i].setBuffer();


	// ������Ʈ �ؽ�ó ����
	// ���� ������Ʈ
	background.setTexture();
	alcy.setTexture();

	// ���� ������Ʈ
	turntable.setTexture();
	speaker.setTexture();
	guitar.setTexture(); 
	arm.setTexture();
	light.setTexture();
	mic.setTexture();
	neon.setTexture();
	note.setTexture();
	for (int i = 0; i < 3; i++) {
		zzz[i].setTexture();
		zzz[i].setDelay(i);  // �� ��ü�� ������ ���� ����
	}

	// UI ������Ʈ
	ui.setTexture();
	info.setTexture();
	white.setTexture();
	for (int i = 0; i < ICON_PART; i++)
		icon[i].setTexture(i);
}


//  (�߿�) �Ʒ��� �� ���� ���� �׷���
void objectOutput() {
	background.objectOut();

	if (playFunc) {
		if(funcNumber == 3)
			neon.objectOut();
		if(funcNumber == 0)
			light.objectOut();
		if(funcNumber == 0 || funcNumber == 1)
			speaker.objectOut();
	}

	alcy.objectOut();

	if (playFunc) {  // ��ɿ� �ش��ϴ� ������Ʈ�� ����Ѵ�.
		if (funcNumber == 0 || funcNumber == 1) 
			turntable.objectOut();

		else if (funcNumber == 2 || funcNumber == 3) {
			guitar.objectOut();
			arm.objectOut();

			if(funcNumber == 2)
				note.objectOut();

			if (funcNumber == 3) {
				light.objectOut();
				mic.objectOut();
			}
		}
	}

	for (int i = 0; i < 3; i++)
		if (zzz[i].zzzTransparent > 0.0)
			zzz[i].objectOut();

	ui.objectOut();

	for (int i = 0; i < ICON_PART; i++) 
		if (icon[i].iconTransparent > 0.0) 
			icon[i].objectOut(i);

	if(info.size > 0.0)
		info.objectOut();

	if (whiteTransparent > 0.0) 
		white.objectOut();
}