#include "../header/mode_header.h"
#include "../object/main_object/Alcy_home.h"
#include "../object/main_object/Cursor_home.h"

void home_mode() {
	fw.add_object(new Alcy_home(alcy_layer, "alcy_home"), alcy_layer);
	fw.add_object(new Cursor_home(ui_layer, "cursor_home"), ui_layer);
}