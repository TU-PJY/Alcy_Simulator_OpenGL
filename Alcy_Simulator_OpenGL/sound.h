#ifndef SOUND_H
#define SOUND_H
#include "config.h"

extern FMOD::System* ssystem;
extern FMOD::Sound* click, *escDown, *escUp, *squeak1, *squeak2, *squeak3, *touch, *tilt, *scroll;
extern FMOD::Channel* channelClick, *channelEscDown, *channelEscUp;
extern FMOD::Channel* channelSqueak, *channelTouch, *channelTilt;
extern FMOD::Channel* channelScroll;
extern void* extradriverdata;
extern FMOD_RESULT result;

extern bool escSoundPlayed;  // �ߺ� ��� ����

void initFmod();

#endif