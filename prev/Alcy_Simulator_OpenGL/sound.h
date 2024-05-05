#ifndef SOUND_H
#define SOUND_H
#include "config.h"

extern FMOD::System* ssystem;

// 사운드
extern FMOD::Sound* click, *escDown, *escUp, * scroll, *menuOpen, *menuClose, *menuClick, *logoSound;
extern FMOD::Sound* squeak1, * squeak2, * squeak3, * touch, * tilt, *breatheIn, *breatheOut;
extern FMOD::Sound* music1, * music2, *music3, *music4, *mainTheme;

// 사운드 채널
extern FMOD::Channel* channelClick, *channelEscDown, *channelEscUp, * channelScroll, *channelMenu;
extern FMOD::Channel* channelSqueak, *channelTouch, *channelTilt, *channelBreathe;
extern FMOD::Channel* channelMusic, * channelTheme, *channelLogo;

extern void* extradriverdata;
extern FMOD_RESULT result;

extern bool escSoundPlayed;  // 종료/정보창 사운드 중복 재생 방지

void initFmod();

#endif