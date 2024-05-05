#ifndef SOUND_H
#define SOUND_H
#include "config.h"

extern FMOD::System* ssystem;

// ����
extern FMOD::Sound* click, *escDown, *escUp, * scroll, *menuOpen, *menuClose, *menuClick, *logoSound;
extern FMOD::Sound* squeak1, * squeak2, * squeak3, * touch, * tilt, *breatheIn, *breatheOut;
extern FMOD::Sound* music1, * music2, *music3, *music4, *mainTheme;

// ���� ä��
extern FMOD::Channel* channelClick, *channelEscDown, *channelEscUp, * channelScroll, *channelMenu;
extern FMOD::Channel* channelSqueak, *channelTouch, *channelTilt, *channelBreathe;
extern FMOD::Channel* channelMusic, * channelTheme, *channelLogo;

extern void* extradriverdata;
extern FMOD_RESULT result;

extern bool escSoundPlayed;  // ����/����â ���� �ߺ� ��� ����

void initFmod();

#endif