#pragma once
#include "HEADER.h"

using ch = FMOD::Channel*;
using sound = FMOD::Sound*;


// system
extern FMOD::System* ssystem;

// bgm channel
extern ch ch_bgm;

// bgms
extern sound home_music;




// effect channels
extern ch ch_effect;

// effect sounds
// ��Ű �� ������ �Ҹ�
extern sound squeak1, squeak2, squeak3;




// init
extern void* extradriverdata;
extern FMOD_RESULT init_result;

// fmod ����
void set_fmod();

// ���� ���
void play_sound(FMOD::Channel* ch, FMOD::Sound* sound);

// ���� ����
void stop_sound(FMOD::Channel* ch);

// ���� �Ͻ�����
void pause_sound(FMOD::Channel* ch);

// ���� �ٽ� ���
void continue_sound(FMOD::Channel* ch);