#include "../header/mode_header.h"

#include "../object/main_object/Alcy_home.h"
#include "../object/main_object/Cursor_home.h"
#include "../object/main_object/BackGround_home.h"
#include "../object/main_object/Shadow.h"
#include "../object/main_object/Front.h"
#include "../object/main_object/Tip_home.h"

#include "../header/sound.h"
#include "../header/Camera.h"

void home_mode() {
	fw.AddMainObj(new BackGround_home(main_layer1, "background_home"), main_layer1);

	fw.AddMainObj(new Shadow(main_layer1, "shadow_home"), main_layer1);

	fw.AddMainObj(new Alcy_home(main_layer2, "alcy_home"), main_layer2);

	fw.AddMainObj(new Tip_home(main_layer3, "tip_home"), main_layer3);

	fw.AddMainObj(new Front(main_layer3, "front_home"), main_layer3);

	fw.AddMainObj(new Cursor_home(main_layer7, "cursor_home"), main_layer7);

	ssys->playSound(home_music, 0, false, &ch_bgm);

	cam.zoom = 2.4;
	cam.zoom_value = 0.8;
}