#ifndef SOUND_H
#define SOUND_H
#include "config.h"

extern FMOD::System* ssystem;

extern FMOD::Sound* click, *escDown, *escUp, * scroll;
extern FMOD::Sound* squeak1, * squeak2, * squeak3, * touch, * tilt, *breatheIn, *breatheOut;
extern FMOD::Sound* music1;

extern FMOD::Channel* channelClick, *channelEscDown, *channelEscUp, * channelScroll;
extern FMOD::Channel* channelSqueak, *channelTouch, *channelTilt, *channelBreathe;
extern FMOD::Channel* channelMusic;

extern void* extradriverdata;
extern FMOD_RESULT result;

extern bool escSoundPlayed;  // 중복 재생 방지

void initFmod();

#endif