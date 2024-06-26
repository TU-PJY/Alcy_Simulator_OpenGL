#ifndef FWL_CONFIG_H
#define FWL_CONFIG_H

#include <array>
#include <vector>
#include <string>
#include "../header/HEADER.h"

#define          USING_FWL
#define          N_MAIN_LAYER 7

#define          USING_SUB_MODE
#define          N_SUB_LAYER 4

#define          USING_FRAME_TIME
#define          USING_FRAME_TIME_OUTSIDE



struct MODELIST {
	// add mode list here...
	std::vector<std::string> MainModeList = 
	{ 
		"home_mode",
		"game_mode", 
		"music_mode1",
		"music_mode2", 
	};

#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// add popup mode list here...
	std::vector<std::string> SubModeList =
	{ 
		"menu_mode", 
		"return_menu_mode",
	};

#endif
#endif
};



class MAIN_CLS {
public:
	virtual void Update() {}
	virtual void CheckCollision() {}
	virtual void Render() {}
	virtual std::string GetTag() const { return {}; }


	////////////////// home mode
	// cursor
	virtual GLfloat get_cursor_x() const { return {}; }
	virtual GLfloat get_cursor_y() const { return {}; }
	virtual GLfloat get_cursor_touch_x() const { return {}; }
	virtual bool get_touch_state() const { return {}; }

	virtual void mouse_left_button_down(int button, int state) {}
	virtual void mouse_left_button_up(int button, int state) {}
	virtual void reset_mouse_state() {}

	virtual void set_cursor_invisible() {}
	virtual void set_cursor_visible() {}

	// alcy 
	virtual std::array<GLfloat, 4> get_touch_zone() const { return {}; }
	virtual std::array<GLfloat, 4> get_squeak_zone() const { return {}; }
	virtual bool get_interaction_available_state() const { return {}; }
	virtual void tell_touch_state(bool state) {}
	virtual void tell_squeak_state(bool state) {}
	virtual int get_head_rotate_state() const { return {}; }

	// tip
	virtual void update_tip_visible(unsigned char KEY) {}


	/////////////////game mode
	// gameboy back
	virtual void set_power_on(bool flag) {}
	virtual bool get_power_state() { return {}; }

	// gameboy
	virtual void gameboy_special_key_down(int KEY, int x, int y) {}
	virtual void gameboy_special_key_up(int KEY, int x, int y) {}
	virtual void gameboy_key_down(unsigned int key, int x, int y) {}
	virtual void gameboy_key_up(unsigned int key, int x, int y) {}

	// game1 controller
	virtual  void set_game1_control_state(bool flag) {}
	virtual void game1_key_down(unsigned int key, int x, int y) {}
	virtual void game1_key_up(unsigned int key, int x, int y) {}
	virtual bool get_game1_play_state() { return {}; }
	virtual int get_score() { return {}; }

	// alcy sprite(game1)
	virtual void set_alcy_sprite_jump_state(bool flag) {}
	virtual void set_alcy_sprite_jump_enable_state(bool flag) {}
	virtual GLfloat get_alcy_sprite_move_speed() { return {}; }
	virtual void increase_alcy_sprite_move_speed(GLfloat speed) {}


	////////////////////

	virtual     ~MAIN_CLS() {}
};






class SUB_CLS {
public:
	virtual void Update() {}
	virtual void CheckCollision() {}
	virtual void Render() {}
	virtual std::string GetTag() const { return {}; }


	//menu mode
	// icon
	virtual size_t get_icon_number() const { return {}; }
	virtual void tell_on_cursor(int idx) {}
	virtual void tell_not_on_cursor(int idx) {}
	virtual void tell_icon_click() {}
	virtual std::array<GLfloat, 4> get_icon_zone(int i) const { return {}; }

	// button
	virtual void tell_on_cursor_button(int idx) {}
	virtual void tell_not_on_cursor_button(int idx) {}
	virtual size_t get_button_number() { return {}; }
	virtual void tell_button_click() {}
	virtual std::array<GLfloat, 4> get_button_zone(int i) { return {}; }

	//info
	virtual void close_info() {}
	virtual void tell_info_is_open() {}
	virtual void tell_info_is_close() {}
	virtual bool get_info_visible_state() { return {}; }

	// cursor
	virtual void mouse_left_button_down(int button, int state) {}



	// return menu
	virtual std::array<GLfloat, 4> get_icon_zone_return_menu() const { return{}; }
	virtual void tell_on_cursor_return_menu() {}
	virtual void tell_not_on_cursor_return_menu() {}
	virtual void tell_icon_click_return_menu() {}

	virtual     ~SUB_CLS() {}
};

#endif