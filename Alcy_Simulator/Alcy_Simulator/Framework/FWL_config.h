#ifndef FWL_CONFIG_H
#define FWL_CONFIG_H

#include <vector>
#include <string>
#include "../header/HEADER.h"

#define          USING_FWL
#define          NUMBER_OF_LAYER 4

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

	virtual std::string get_tag() { return {}; }

	// cursor
	virtual GLfloat get_cursor_x() { return {}; }
	virtual GLfloat get_cursor_y() { return {}; }


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

	virtual std::string get_tag() { return {}; }

	virtual     ~POPUP_FUNCTION() {}
};

#endif
#endif

#endif