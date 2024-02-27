#include "sound.h"

FMOD::System* ssystem;

FMOD::Sound* click, * escDown, * escUp, * scroll, *menuOpen, *menuClose, *menuClick, *logoSound;
FMOD::Sound* squeak1, * squeak2, * squeak3, * touch, * tilt, *breatheIn, *breatheOut;
FMOD::Sound* music1, *music2, *music3, *music4, *mainTheme;

FMOD::Channel* channelClick = 0, *channelEscDown = 0, *channelEscUp = 0, *channelMenu = 0;
FMOD::Channel* channelSqueak = 0, *channelTouch = 0, *channelTilt = 0, *channelBreathe = 0, * channelScroll = 0;
FMOD::Channel* channelMusic = 0, * channelTheme = 0, * channelLogo = 0;

void* extradriverdata = 0;
FMOD_RESULT result;

bool escSoundPlayed;  // 중복 재생 방지

void initFmod() {
	ssystem->createSound("res//sound//UI//button_click.wav", FMOD_DEFAULT, 0, &click);
	ssystem->createSound("res//sound//UI//button_down.wav", FMOD_DEFAULT, 0, &escDown);
	ssystem->createSound("res//sound//UI//button_up.wav", FMOD_DEFAULT, 0, &escUp);
	ssystem->createSound("res//sound//UI//scroll.wav", FMOD_DEFAULT, 0, &scroll);
	ssystem->createSound("res//sound//UI//menu_open.wav", FMOD_DEFAULT, 0, &menuOpen);
	ssystem->createSound("res//sound//UI//menu_close.wav", FMOD_DEFAULT, 0, &menuClose);
	ssystem->createSound("res//sound//UI//menu_click.wav", FMOD_DEFAULT, 0, &menuClick);

	ssystem->createSound("res//sound//alcy//squeak1.mp3", FMOD_DEFAULT, 0, &squeak1);
	ssystem->createSound("res//sound//alcy//squeak2.mp3", FMOD_DEFAULT, 0, &squeak2);
	ssystem->createSound("res//sound//alcy//squeak3.mp3", FMOD_DEFAULT, 0, &squeak3);
	ssystem->createSound("res//sound//alcy//breathe_in.wav", FMOD_DEFAULT, 0, &breatheIn);
	ssystem->createSound("res//sound//alcy//breathe_out.wav", FMOD_DEFAULT, 0, &breatheOut);
	ssystem->createSound("res//sound//alcy//touch.wav", FMOD_LOOP_NORMAL, 0, &touch);
	ssystem->createSound("res//sound//alcy//tilt.wav", FMOD_DEFAULT, 0, &tilt);

	ssystem->createSound("res//sound//music//music1.mp3", FMOD_DEFAULT, 0, &music1);
	ssystem->createSound("res//sound//music//music2.mp3", FMOD_DEFAULT, 0, &music2);
	ssystem->createSound("res//sound//music//guitar1.mp3", FMOD_DEFAULT, 0, &music3);
	ssystem->createSound("res//sound//music//guitar2.mp3", FMOD_DEFAULT, 0, &music4);
	ssystem->createSound("res//sound//music//main_theme.mp3", FMOD_LOOP_NORMAL, 0, &mainTheme);
}