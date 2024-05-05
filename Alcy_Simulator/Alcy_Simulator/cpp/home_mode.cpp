#include "../header/mode_header.h"
#include "../object/main_object/Alcy.h"
#include "../object/main_object/Cursor.h"

void home_mode() {
	fw.add_object(new Alcy(alcy_layer, "alcy"), alcy_layer);
	fw.add_object(new Cursor(ui_layer, "cursor"), ui_layer);
}