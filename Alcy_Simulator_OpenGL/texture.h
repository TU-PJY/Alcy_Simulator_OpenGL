#ifndef TEXTURE_H
#define TEXTURE_H

extern unsigned char* texture_data;

// ui
extern unsigned int cursor[3];  // 배경, 커서
extern unsigned int bar;  //메뉴바
extern unsigned int uiIcon[3];  // 아이콘
extern unsigned int tip;  // 팁
extern unsigned int title;  // 타이틀

// 알키
extern unsigned int alcyTail, alcyBody, alcyHair, alcyHead[4];  // 알키 파츠
extern unsigned int eye[5], dot[3], eyeClose[3], brow[3], blink[3]; // 알키 얼굴 파츠


#endif