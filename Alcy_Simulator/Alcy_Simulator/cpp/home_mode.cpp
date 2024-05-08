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
	fw.add_object(new BackGround_home(layer1, "background_home"), layer1);

	fw.add_object(new Shadow(layer1, "shadow_home"), layer1);

	fw.add_object(new Alcy_home(layer2, "alcy_home"), layer2);

	fw.add_object(new Tip_home(layer3, "tip_home"), layer3);

	fw.add_object(new Front(layer3, "front_home"), layer3);

	fw.add_object(new Cursor_home(layer7, "cursor_home"), layer7);

	ssys->playSound(home_music, 0, false, &ch_bgm);

	cam.zoom = 2.4;
	cam.zoom_value = 0.8;
}