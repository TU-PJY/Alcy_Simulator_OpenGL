// OpenGL 기능 함수
#pragma once
#include "HEADER.h"

void convert_cursor_position(int x, int y);

// home mode
void home_mode_mouse_button(int button, int state, int x, int y);
void home_mode_mouse_motion(int x, int y);
void home_mode_mouse_passive_motion(int x, int y);
void home_mode_wheel(int button, int dir, int x, int y);
void home_mode_key_down(unsigned char KEY, int x, int y);
void home_mode_key_up(unsigned char KEY, int x, int y);

// game mode
void game_mode_mouse_button(int button, int state, int x, int y);
void game_mode_key_down(unsigned char KEY, int x, int y);
void game_mode_special_key_down(int KEY, int x, int y);
void game_mode_special_key_up(int KEY, int x, int y);

// menu mode
void menu_mode_mouse_button(int button, int state, int x, int y);
void menu_mode_mouse_motion(int x, int y);
void menu_mode_mouse_passive_motion(int x, int y);
void menu_mode_wheel(int button, int dir, int x, int y);
void menu_mode_key_down(unsigned char KEY, int x, int y);
void menu_mode_key_up(unsigned char KEY, int x, int y);

// return menu mode
void return_menu_mode_mouse_button(int button, int state, int x, int y);
void return_menu_mode_mouse_motion(int x, int y);
void return_menu_mode_mouse_passive_motion(int x, int y);
void return_menu_mode_wheel(int button, int dir, int x, int y);
void return_menu_mode_key_down(unsigned char KEY, int x, int y);
void return_menu_mode_key_up(unsigned char KEY, int x, int y);
