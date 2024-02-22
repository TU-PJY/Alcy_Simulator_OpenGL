#include "sound.h"

FMOD::System* ssystem;

FMOD::Sound* click, * escDown, * escUp, * scroll;
FMOD::Sound* squeak1, * squeak2, * squeak3, * touch, * tilt, *breatheIn, *breatheOut;
FMOD::Sound* music1, *music2, *mainTheme;

FMOD::Channel* channelClick = 0, *channelEscDown = 0, *channelEscUp = 0;
FMOD::Channel* channelSqueak = 0, *channelTouch = 0, *channelTilt = 0, *channelBreathe = 0, * channelScroll = 0;
FMOD::Channel* channelMusic = 0, * channelTheme = 0;

void* extradriverdata = 0;
FMOD_RESULT result;

bool escSoundPlayed;  // 중복 재생 방지

void initFmod() {
	ssystem->init(64, FMOD_INIT_NORMAL, extradriverdata);

	ssystem->createSound("res//sound//button_click.wav", FMOD_DEFAULT, 0, &click);
	ssystem->createSound("res//sound//button_down.wav", FMOD_DEFAULT, 0, &escDown);
	ssystem->createSound("res//sound//button_up.wav", FMOD_DEFAULT, 0, &escUp);

	ssystem->createSound("res//sound//squeak1.mp3", FMOD_DEFAULT, 0, &squeak1);
	ssystem->createSound("res//sound//squeak2.mp3", FMOD_DEFAULT, 0, &squeak2);
	ssystem->createSound("res//sound//squeak3.mp3", FMOD_DEFAULT, 0, &squeak3);

	ssystem->createSound("res//sound//breathe_in.wav", FMOD_DEFAULT, 0, &breatheIn);
	ssystem->createSound("res//sound//breathe_out.wav", FMOD_DEFAULT, 0, &breatheOut);

	ssystem->createSound("res//sound//touch.wav", FMOD_LOOP_NORMAL, 0, &touch);
	ssystem->createSound("res//sound//tilt.wav", FMOD_DEFAULT, 0, &tilt);

	ssystem->createSound("res//sound//scroll.wav", FMOD_DEFAULT, 0, &scroll);

	ssystem->createSound("res//sound//music//music1.mp3", FMOD_DEFAULT, 0, &music1);
	ssystem->createSound("res//sound//music//music2.mp3", FMOD_DEFAULT, 0, &music2);

	ssystem->createSound("res//sound//music//main_theme.mp3", FMOD_LOOP_NORMAL, 0, &mainTheme);

	if(INTRO == 0)  // 인트로를 비활성화 했을 경우 메인 음악을 바로 킨다
	    ssystem->playSound(mainTheme, 0, false, &channelTheme);  // 메인 브금
}