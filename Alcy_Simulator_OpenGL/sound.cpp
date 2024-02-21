#include "sound.h"

FMOD::System* ssystem;
FMOD::Sound* click, * escDown, * escUp, * scroll;
FMOD::Sound* squeak1, * squeak2, * squeak3, * touch, * tilt, *breatheIn, *breatheOut;
FMOD::Channel* channelClick = 0, *channelEscDown = 0, *channelEscUp = 0;
FMOD::Channel* channelSqueak = 0, *channelTouch = 0, *channelTilt = 0, *channelBreathe = 0;
FMOD::Channel* channelScroll = 0;
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

	ssystem->createSound("res//sound//scroll.wav", FMOD_DEFAULT, 0, & scroll);
}