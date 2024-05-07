#include "../header/mode_header.h"

#include "../object/main_object/Alcy_home.h"
#include "../object/main_object/Cursor_home.h"
#include "../object/main_object/BackGround_home.h"
#include "../object/main_object/Shadow.h"
#include "../object/main_object/Front.h"
#include "../object/main_object/Tip.h"

#include "../header/sound.h"
#include "../header/Camera.h"

void home_mode() {
	fw.add_object(new BackGround_home(background_layer, "background_home"), background_layer);

	fw.add_object(new Shadow(prop_layer1, "shadow_home"), prop_layer1);

	fw.add_object(new Alcy_home(alcy_layer, "alcy_home"), alcy_layer);

	fw.add_object(new Tip(ui_layer, "tip"), ui_layer);

	fw.add_object(new Front(ui_layer, "front_home"), ui_layer);

	fw.add_object(new Cursor_home(cursor_layer, "cursor_home"), cursor_layer);

	ssys->playSound(home_music, 0, false, &ch_bgm);

	cam.zoom = 2.4;
	cam.zoom_value = 0.8;

}