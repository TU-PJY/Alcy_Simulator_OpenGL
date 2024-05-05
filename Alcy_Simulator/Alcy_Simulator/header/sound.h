#pragma once
#include "HEADER.h"

extern FMOD::System* ssys;
extern FMOD::System* ssys_ui;


// bgm sounds
extern FMOD::Channel* ch_bgm;
extern FMOD::Sound* home_music;


// alcy sounds
extern FMOD::Channel* ch_alcy;
extern FMOD::Sound* squeak1, *squeak2, *squeak3;
extern FMOD::Sound* touch, *tilt, *welcome;


//ui sounds
extern FMOD::Channel* ch_ui;
extern FMOD::Sound* scroll_sound;
extern FMOD::Sound* menu_open, * menu_close;



extern FMOD_RESULT f_result;
extern void* extdvdata;

void load_sound_file();