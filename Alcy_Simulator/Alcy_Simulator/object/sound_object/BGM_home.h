#pragma once
#include "../../header/sound.h"

// È¨ ¸ðµå ¹è°æÀ½¾Ç °´Ã¼
class BGM_home : public FUNCTION {
private:
	int layer{};
	std::string tag;

	fm ssys;
	ch ch{};
	void* extdvdata{};
	FMOD_RESULT result;

	sound bgm;


public:
	void update() {}
	void render() {}
	void check_collision() {}
	void check_delete() {}

	BGM_home(int l, std::string str) {
		layer = l;
		tag = str;

		result = FMOD::System_Create(&ssys);
		if (result != FMOD_OK)	exit(0);
		ssys->init(32, FMOD_INIT_NORMAL, extdvdata);

		ssys->createSound("res//sound//music//home_music.mp3", FMOD_LOOP_NORMAL, 0, &bgm);

		ssys->playSound(bgm, 0, false, &ch);
	}

	~BGM_home() {
		ssys->release();
	}
};