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

//#define          USING_DEBUG_MESSEGE
#define          USING_FRAME_TIME
#define          USING_FRAME_TIME_OUTSIDE



class MODELIST {
public:
	// add mode list here...
	std::vector<std::string> MainModeList = 
	{ "home_mode",
	"music_mode1",
	"music_mode2",
	"game_mode", };

#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// add popup mode list here...
	std::vector<std::string> SubModeList =
	{ "menu_mode", };

#endif
#endif
};



class MAIN_CLS {
public:
	virtual void Update() {}
	virtual void CheckCollision() {}
	virtual void Render() {}
	virtual void CheckDelete() {}
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

	// tip
	virtual void update_tip_visible(unsigned char KEY) {}

	////////////////////

	virtual     ~MAIN_CLS() {}
};



class SUB_CLS {
public:
	virtual void Update() {}
	virtual void CheckCollision() {}
	virtual void Render() {}
	virtual void CheckDelete() {}
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

	virtual     ~SUB_CLS() {}
};

#endif