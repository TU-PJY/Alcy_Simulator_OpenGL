#include "sound.h"

FMOD::System* ssystem;
FMOD::Sound* click, *escDown, *escUp;
FMOD::Channel* channelClick = 0, *channelEscDown = 0, *channelEscUp = 0;
void* extradriverdata = 0;
FMOD_RESULT result;

bool escSoundPlayed;  // 중복 재생 방지

void initFmod() {
	ssystem->init(64, FMOD_INIT_NORMAL, extradriverdata);

	ssystem->createSound("res//sound//button_click.wav", FMOD_DEFAULT, 0, &click);
	ssystem->createSound("res//sound//button_down.wav", FMOD_DEFAULT, 0, &escDown);
	ssystem->createSound("res//sound//button_up.wav", FMOD_DEFAULT, 0, &escUp);
}