#include "../header/mode_header.h"

#include "../object/popup_object/Menu.h"
#include "../object/popup_object/Cursor_menu.h"
#include "../object/popup_object/Back.h"

void menu_mode() {
	fw.add_popup_object(new Back(popup_menu_layer, "back"), popup_menu_layer);
	fw.add_popup_object(new Menu(popup_menu_layer, "menu"), popup_menu_layer);
	fw.add_popup_object(new Cursor_menu(popup_cursor_layer, "cursor_menu"), popup_cursor_layer);
}