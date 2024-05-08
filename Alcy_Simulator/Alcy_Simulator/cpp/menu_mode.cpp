#include "../header/mode_header.h"

#include "../object/popup_object/Menu.h"
#include "../object/popup_object/Cursor_menu.h"
#include "../object/popup_object/Back.h"

void menu_mode() {
	fw.add_popup_object(new Back(popup_layer1, "back"), popup_layer1);
	fw.add_popup_object(new Menu(popup_layer1, "menu"), popup_layer1);
	fw.add_popup_object(new Cursor_menu(popup_layer4, "cursor_menu"), popup_layer4);
}