#pragma once
#include "HEADER.h"

using fm = FMOD::System*;
using ch = FMOD::Channel*;
using sound = FMOD::Sound*;

// fmod init template
//result = FMOD::System_Create(&ssys);
//if (result != FMOD_OK)	exit(0);
//ssys->init(32, FMOD_INIT_NORMAL, extdvdata);

// fmod variable template
//fm ssys;
//ch ch{};
//void* extdvdata{};
//FMOD_RESULT result;