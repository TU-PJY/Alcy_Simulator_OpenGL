#include "../header/mode_header.h"

#include "../object/popup_object/Menu.h"
#include "../object/popup_object/Cursor_menu.h"
#include "../object/popup_object/Back.h"

void menu_mode() {
	fw.add_popup_object(new Back(layer1, "back"), layer1);
	fw.add_popup_object(new Menu(layer1, "menu"), layer1);
	fw.add_popup_object(new Cursor_menu(layer3, "cursor_menu"), layer3);
}