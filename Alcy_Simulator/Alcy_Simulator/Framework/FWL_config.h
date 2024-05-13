#ifndef FWL_CONFIG_H
#define FWL_CONFIG_H

#include <array>
#include <vector>
#include <string>
#include "../header/HEADER.h"

#define          USING_FWL
#define          NUMBER_OF_LAYER 7

#define          USING_POPUP_MODE
#define          NUMBER_OF_POPUP_LAYER 4

//#define          USING_DEBUG_MESSEGE
#define          USING_FRAME_TIME



class MODELIST {
public:
	// add mode list here...
	std::vector<std::string> mode_list = 
	{
	"home_mode",
	"music_mode1",
	"music_mode2",
	"game_mode",

	};

#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER

	// add popup mode list here...
	std::vector<std::string> popup_mode_list =
	{ "menu_mode", };

#endif
#endif
};



class FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete() {}

	virtual std::string get_tag() const { return {}; }

	////////////////// home mode
	// 
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

	// tip
	virtual void update_tip_visible(unsigned char KEY) {}

	////////////////////

	virtual     ~FUNCTION() {}
};



#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER

class POPUP_FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete() {}

	virtual std::string get_tag() const { return {}; }

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
	virtual size_t get_button_number() {return {};}
	virtual void tell_button_click() {}
	virtual std::array<GLfloat, 4> get_button_zone(int i) { return {}; }

	//info
	virtual void close_info() {}
	virtual void tell_info_is_open() {}
	virtual void tell_info_is_close() {}
	virtual bool get_info_visible_state() { return {}; }

	// cursor
	virtual void mouse_left_button_down(int button, int state) {}


	virtual     ~POPUP_FUNCTION() {}
};

#endif
#endif

#endif