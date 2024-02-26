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
#include "main2.h"

// 이미지 출력 cpp

void initObject() {
	// 버퍼 초기화
	background.setBuffer();  // 배경 초기화
	alcy.setBuffer();  // 알키 초기화
	ui.setBuffer();  // ui 초기화
	turntable.setBuffer();  // 턴테이블 초기화
	speaker.setBuffer(); // 스피커 초기화
	guitar.setBuffer(); // 기타 초기화
	arm.setBuffer(); // 팔 초기화
	light.setBuffer();  // 빛 초기화
	white.setBuffer();  // 흰 배경 초기화

	for (int i = 0; i < ICON_PART; i++)  // 메뉴 아이콘 초기화
		icon[i].setBuffer();
	for (int i = 0; i < 3; i++)  // zzz오브젝트 초기화
		zzz[i].setBuffer();


	// 텍스처 설정
	background.setTexture();  // 배경
	alcy.setTexture();  // 알키
	ui.setTexture();  // UI
	turntable.setTexture();
	speaker.setTexture();
	guitar.setTexture(); 
	arm.setTexture();
	white.setTexture();
	light.setTexture();

	for (int i = 0; i < ICON_PART; i++)
		icon[i].setTexture(i);
	for (int i = 0; i < 3; i++) {// zzz오브젝트
		zzz[i].setTexture();
		zzz[i].setDelay(i);
	}
}


//  아래로 갈 수록 위 레이어에 그려짐
void objectOutput() {
	// 배경
	background.setObject();

	// 스피커
	if (playFunc && (funcNumber == 0 || funcNumber == 1)) 
		speaker.setObject();

	// Alcy
	for (int i = 0; i < ALCY_PART; i++)
		alcy.setObject(i);

	if (playFunc) {
		// 턴테이블
		if (funcNumber == 0 || funcNumber == 1) 
			turntable.setObject();

		// 기타, 팔
		else if (funcNumber == 2 || funcNumber == 3) {
			guitar.setObject();
			arm.setObject();

			if (funcNumber == 3)  // 빛
				light.setObject();
		}
	}

	// ZZZ 이미지
	for (int i = 0; i < 3; i++)
		if (zzz[i].zzzTransparent > 0.0)
			zzz[i].setObject();

	// UI
	for (int i = 0; i < UI_PART; i++)
		ui.setObject(i);

	// 메뉴 아이콘
	for (int i = 0; i < ICON_PART; i++) 
		if (icon[i].iconTransparent > 0.0) 
			icon[i].setObject(i);

	//흰 배경
	if (whiteTransparent > 0.0) 
		white.setObject();
}