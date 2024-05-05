#include "../header/sound.h"

FMOD::System* ssystem;

// bgm channels
ch ch_bgm{};

// bgms
sound home_music;



// effect channels
ch ch_effect{};

// effect sounds
// ��Ű �� ������ �Ҹ�
sound squeak1, squeak2, squeak3;




void* extradriverdata{};
FMOD_RESULT init_result;


void load_bgm() {
	ssystem->createSound("res//sound//music//home_music.mp3", FMOD_LOOP_NORMAL, 0, &home_music);
}

void load_effect() {
	ssystem->createSound("res//sound//alcy//squeak1.mp3", FMOD_DEFAULT, 0, &squeak1);
	ssystem->createSound("res//sound//alcy//squeak2.mp3", FMOD_DEFAULT, 0, &squeak2);
	ssystem->createSound("res//sound//alcy//squeak3.mp3", FMOD_DEFAULT, 0, &squeak3);
}

// fmod ����
void set_fmod() {
	init_result = FMOD::System_Create(&ssystem);
	if (init_result != FMOD_OK)
		exit(0);

	ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata);

	load_bgm();
	load_effect();
}

// ���� ���
void play_sound(FMOD::Channel *ch, FMOD::Sound* sound) {
	ssystem->playSound(sound, 0, false, &ch);
}

// ���� ����
void stop_sound(FMOD::Channel* ch) {
	ch->stop();
}

// ���� �Ͻ�����
void pause_sound(FMOD::Channel* ch) {
	ch->setVolume(0.0);
}

// ���� �ٽ� ���
void continue_sound(FMOD::Channel* ch) {
	ch->setVolume(1.0);
}