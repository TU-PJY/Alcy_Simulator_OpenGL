#include "../header/mode_header.h"

#include "../object/popup_object/Menu.h"
#include "../object/popup_object/Cursor_menu.h"
#include "../object/popup_object/Back.h"

void menu_mode() {
	fw.AddSubObj(new Back(sub_layer1, "back"), sub_layer1);
	fw.AddSubObj(new Menu(sub_layer1, "menu"), sub_layer1);
	fw.AddSubObj(new Cursor_menu(sub_layer4, "cursor_menu"), sub_layer4);
}