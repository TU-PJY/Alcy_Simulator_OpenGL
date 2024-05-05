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
// 알키 코 누르기 소리
extern sound squeak1, squeak2, squeak3;




// init
extern void* extradriverdata;
extern FMOD_RESULT init_result;

// fmod 세팅
void set_fmod();

// 사운드 재생
void play_sound(FMOD::Channel* ch, FMOD::Sound* sound);

// 사운드 중지
void stop_sound(FMOD::Channel* ch);

// 사운드 일시정지
void pause_sound(FMOD::Channel* ch);

// 사운드 다시 재생
void continue_sound(FMOD::Channel* ch);