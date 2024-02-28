#include "config.h"  // 중요 헤더 파일
#include "transform.h"  // 변환
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

// 오브젝트 설정 및 출력

void initObject() {  // 오브젝트 설정
	// 오브젝트 버퍼 설정
	// 메인 오브젝트
	background.setBuffer(); 
	alcy.setBuffer();

	// 서브 오브젝트
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

	// UI 오브젝트
	ui.setBuffer();
	info.setBuffer();
	white.setBuffer();
	for (int i = 0; i < ICON_PART; i++) 
		icon[i].setBuffer();


	// 오브젝트 텍스처 설정
	// 메인 오브젝트
	background.setTexture();
	alcy.setTexture();

	// 서브 오브젝트
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
		zzz[i].setDelay(i);  // 각 객체의 딜레이 시작 설정
	}

	// UI 오브젝트
	ui.setTexture();
	info.setTexture();
	white.setTexture();
	for (int i = 0; i < ICON_PART; i++)
		icon[i].setTexture(i);
}


//  (중요) 아래로 갈 수록 위에 그려짐
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

	if (playFunc) {  // 기능에 해당하는 오브젝트만 출력한다.
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