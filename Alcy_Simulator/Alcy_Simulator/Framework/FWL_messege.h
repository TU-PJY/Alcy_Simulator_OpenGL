#ifndef FWL_MESSEGE_H
#define FWL_MESSEGE_H
#include <iostream>
#include <vector>
#include "FWL_config.h"

#ifdef USING_FWL
#if NUMBER_OF_LAYER

class FWL_MESSEGE {
private:
	std::vector<std::string> mode_list;
	std::string next_mode_name{};
	std::string current_mode_name{};
	std::string prev_mode_name{};
	bool is_pause{};

	#ifdef USING_POPUP_MODE
	#if NUMBER_OF_POPUP_LAYER
	std::vector<std::string> popup_mode_list; 
	std::string next_popup_mode_name{};
	std::string current_popup_mode_name{};
	std::string prev_popup_mode_name{};
	#endif
	#endif


public:
	void save_next_name(std::string name) { next_mode_name = name; }
	void save_curr_name(std::string name) { current_mode_name = name; }
	void save_prev_mode_name(std::string name) { prev_mode_name = name; }

	void save_next_popup_name(std::string name) { next_popup_mode_name = name; }
	void save_curr_popup_name(std::string name) { current_popup_mode_name = name; }
	void save_prev_popup_mode_name(std::string name) { prev_popup_mode_name = name; }

	void save_is_pause(bool b) { is_pause = b; }


	void process_popup_init_messege() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		if (is_pause)
			std::cout << "Main mode is paused." << std::endl;
		std::cout << "Popup is prepared." << std::endl << std::endl;
#endif
	}

	void process_popup_close_messege() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "Popup is end." << std::endl << std::endl;
#endif
	}

	void process_FWL_init_messege() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "FWL is prepared." << std::endl << std::endl;
#endif
	}

	void process_mode_change_messege() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "Mode changed '" << prev_mode_name << "' -> '" << current_mode_name << "'." << std::endl << std::endl;
#endif
	}

	void process_popup_mode_change_messege() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "Popup mode changed '" << prev_popup_mode_name << "' -> '" << current_popup_mode_name << "'." << std::endl << std::endl;
#endif
	}




	void process_err(std::string err_type, std::string invalid_mode = "") {
#ifdef USING_DEBUG_MESSEGE

		std::cout << std::endl << "==========[ERROR OCCURED]==========" << std::endl << std::endl;
		std::cout << "Error type: " << err_type << std::endl << std::endl;

		// init error
		if (err_type == "FWL init error::Reapeted init")
			std::cout << "Repeated FWL initialization attempt has been detected." << std::endl << std::endl;

		else if (err_type == "FWL init error::Invalid mode") {
			std::cout << "Invalid mode initialization attempt has been detected." << std::endl << std::endl;
			std::cout << "Invalid mode: '" << invalid_mode << "'" << std::endl << std::endl;
			std::cout << "[List of valid modes]" << std::endl;

			for (auto& s : mode_list)
				std::cout << "" << s << "" << std::endl;
		}

		else if (err_type == "FWL init error::Layer out of bounds::main container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At mode start: '" << next_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL init error::Invalid initialization") {
			std::cout << "FWL is not prepared yet." << std::endl;
			std::cout << "Execute FWL's function 'init(func startmode, std::string modename)' first." << std::endl;
		}


		// mode error
		else if (err_type == "FWL mode error::Invalid mode") {
			std::cout << "Invalid mode change attempt has been detected." << std::endl << std::endl;
			std::cout << "At mode change: '" << current_mode_name << "' -> invalid mode '" << invalid_mode << "'" << std::endl << std::endl;
			std::cout << "[List of valid modes]" << std::endl;

			for (auto& s : mode_list)
				std::cout << "" << s << "" << std::endl;
		}

		else if (err_type == "FWL mode error::Same mode as current mode") {
			std::cout << "An attempt to change to the same mode as the current mode has been detected." << std::endl << std::endl;
			std::cout << "At mode: '" << current_mode_name << "'" << std::endl;
		}


		// object addition error
		else if (err_type == "FWL object addition error::Layer out of bounds::temp container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At mode change: '" << current_mode_name << "' -> '" << next_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL object addition error::Layer out of bounds::main container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At mode: '" << current_mode_name << "'" << std::endl;
		}


		// object deletion error
		else if (err_type == "FWL object deletion error::Layer out of bounds") {
			std::cout << "Deletion of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At mode: '" << current_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL object deletion error::Can not find object") {
			std::cout << "Deletion of unknown object attempt has been detected." << std::endl << std::endl;
			std::cout << "At mode: '" << current_mode_name << "'" << std::endl;
		}


		// ptr error
		else if (err_type == "FWL ptr error::Layer out of bounds") {
			std::cout << "Connection of ptr attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At mode: '" << current_mode_name << "'" << std::endl;
		}


		// layer size errpr
		else if (err_type == "FWL layer size error::Layer out of bounds") {
			std::cout << "Obtaining layer size attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At mode: '" << current_mode_name << "'" << std::endl;
		}


		// sweep layer error
		else if (err_type == "FWL layer sweep error::Layer out of bounds") {
			std::cout << "Layer sweep attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At mode: '" << current_mode_name << "'" << std::endl;
		}

#endif
		exit(1);
	}




	void process_popup_err(std::string err_type, std::string invalid_mode = "") {
#ifdef USING_DEBUG_MESSEGE

		std::cout << std::endl << "==========[ERROR OCCURED]==========" << std::endl << std::endl;
		std::cout << "Error type: " << err_type << std::endl << std::endl;

		// init error
		if (err_type == "FWL popup init error::Reapeted init")
			std::cout << "Repeated popup initialization attempt has been detected." << std::endl << std::endl;

		else if (err_type == "FWL popup init error::Invalid mode") {
			std::cout << "Invalid mode initialization attempt has been detected." << std::endl << std::endl;
			std::cout << "Invalid popup mode: '" << invalid_mode << "'" << std::endl << std::endl;
			std::cout << "[List of valid popup modes]" << std::endl;

			for (auto& s : popup_mode_list)
				std::cout << "" << s << "" << std::endl;
		}

		else if (err_type == "FWL popup init error::Layer out of bounds::main container") {
			std::cout << "Addition of popup object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At popup mode start: '" << next_popup_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL popup init error::Invalid initialization") {
			std::cout << "Popup is not prepared yet." << std::endl;
			std::cout << "Execute popup initialization function 'init_popup(func startmode, std::string modename)' first." << std::endl;
		}


		// mode error
		else if (err_type == "FWL popup mode error::Invalid mode") {
			std::cout << "Invalid mode change attempt has been detected." << std::endl << std::endl;
			std::cout << "At popup mode change: '" << current_popup_mode_name << "' -> invalid mode '" << invalid_mode << "'" << std::endl << std::endl;
			std::cout << "[List of valid popup modes]" << std::endl;

			for (auto& s : popup_mode_list)
				std::cout << "" << s << "" << std::endl;
		}

		else if (err_type == "FWL popup mode error::Same mode as current mode") {
			std::cout << "An attempt to change to the same mode as the current mode has been detected." << std::endl << std::endl;
			std::cout << "At popup mode: '" << current_popup_mode_name << "'" << std::endl;
		}


		// object addition error
		else if (err_type == "FWL popup object addition error::Layer out of bounds::temp popup container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At popup mode change: '" << current_popup_mode_name << "' -> '" << next_popup_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL popup object addition error::Layer out of bounds::main popup container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At popup mode: '" << current_popup_mode_name << "'" << std::endl;
		}


		// object deletion error
		else if (err_type == "FWL popup object deletion error::Layer out of bounds") {
			std::cout << "Deletion of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At popup mode: '" << current_popup_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL popup object deletion error::Can not find object") {
			std::cout << "Deletion of unknown object attempt has been detected." << std::endl << std::endl;
			std::cout << "At popup mode: '" << current_popup_mode_name << "'" << std::endl;
		}


		// ptr error
		else if (err_type == "FWL popup ptr error::Layer out of bounds") {
			std::cout << "Connection of ptr attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At popup mode: '" << current_popup_mode_name << "'" << std::endl;
		}


		// layer size errpr
		else if (err_type == "FWL popup layer size error::Layer out of bounds") {
			std::cout << "Obtaining layer size attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At popup mode: '" << current_popup_mode_name << "'" << std::endl;
		}


		// sweep layer error
		else if (err_type == "FWL popup layer sweep error::Layer out of bounds") {
			std::cout << "Layer sweep attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "At popup mode: '" << current_popup_mode_name << "'" << std::endl;
		}

#endif
		exit(1);
	}




	FWL_MESSEGE() {
		MODELIST m;

		mode_list = m.mode_list;

#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER
		popup_mode_list = m.popup_mode_list;
#endif
#endif
	}
};

#endif
#endif

#endif
