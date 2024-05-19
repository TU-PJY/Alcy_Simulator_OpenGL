#include "../header/sound.h"

FMOD::System* ssys;
FMOD::System* ssys_ui;
FMOD::System* ssys_game;


// bgm sounds
FMOD::Channel* ch_bgm{};
FMOD::Sound* home_music;


// alcy sounds
FMOD::Channel* ch_alcy{};
FMOD::Sound* squeak1, *squeak2, *squeak3;
FMOD::Sound* touch, * tilt, * welcome;


//ui sounds
FMOD::Channel* ch_ui{};
FMOD::Sound* scroll_sound;
FMOD::Sound* menu_open, * menu_close;
FMOD::Sound* tip_click;
FMOD::Sound* menu_click;

// game effect sounds
FMOD::Channel* ch_game_ef;
FMOD::Sound* boot_sound;
FMOD::Sound* selector_sound;


FMOD_RESULT f_result;
void* extdvdata{};

void load_sound_file() {
	f_result = FMOD::System_Create(&ssys);
	if (f_result != FMOD_OK)	exit(0);
	ssys->init(32, FMOD_INIT_NORMAL, extdvdata);

	f_result = FMOD::System_Create(&ssys_ui);
	if (f_result != FMOD_OK)	exit(0);
	ssys_ui->init(32, FMOD_INIT_NORMAL, extdvdata);

	f_result = FMOD::System_Create(&ssys_game);
	if (f_result != FMOD_OK)	exit(0);
	ssys_game->init(32, FMOD_INIT_NORMAL, extdvdata);


	ssys->createSound("res//sound//music//home_music.ogg", FMOD_LOOP_NORMAL, 0, &home_music);

	ssys->createSound("res//sound//alcy//squeak1.ogg", FMOD_DEFAULT, 0, &squeak1);
	ssys->createSound("res//sound//alcy//squeak2.ogg", FMOD_DEFAULT, 0, &squeak2);
	ssys->createSound("res//sound//alcy//squeak3.ogg", FMOD_DEFAULT, 0, &squeak3);
	ssys->createSound("res//sound//alcy//touch.ogg", FMOD_LOOP_NORMAL, 0, &touch);
	ssys->createSound("res//sound//alcy//tilt.ogg", FMOD_DEFAULT, 0, &tilt);
	ssys->createSound("res//sound//alcy//welcome.ogg", FMOD_DEFAULT, 0, &welcome);

	ssys_ui->createSound("res//sound//UI//scroll.wav", FMOD_DEFAULT, 0, &scroll_sound);
	ssys_ui->createSound("res//sound//UI//menu_open.ogg", FMOD_DEFAULT, 0, &menu_open);
	ssys_ui->createSound("res//sound//UI//menu_close.ogg", FMOD_DEFAULT, 0, &menu_close);
	ssys_ui->createSound("res//sound//UI//button_click.ogg", FMOD_DEFAULT, 0, &tip_click);
	ssys_ui->createSound("res//sound//UI//menu_click.ogg", FMOD_DEFAULT, 0, &menu_click);

	ssys_game->createSound("res//sound//game//boot.ogg", FMOD_DEFAULT, 0, &boot_sound);
	ssys_game->createSound("res//sound//game//select.ogg", FMOD_DEFAULT, 0, &selector_sound);
}