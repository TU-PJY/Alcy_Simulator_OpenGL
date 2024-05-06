#ifndef FWL_H
#define FWL_H
#pragma warning(disable: 28020)

#include <algorithm>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

#include "FWL_config.h"
#include "FWL_messege.h"

#ifdef USING_FWL
#if NUMBER_OF_LAYER

class FWL {
private:
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> main_cont{};
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> temp_cont{};

	std::vector<std::string> mode_list;
	std::string prev_mode_name{};

	std::string current_mode{};

	bool framework_start{};
	bool framework_initialization{};
	bool changing_mode{};
	bool pause{};

#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER
	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> popup_cont{};
	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> temp_popup_cont{};

	bool popup_mode_start{};
	bool popup_initializtion{};
	bool popup_mode_end{};
	std::vector<std::string> popup_mode_list;
#endif
#endif

	typedef void (*func)(void);

// frame time
#ifdef USING_FRAME_TIME
	clock_t   start_time{}, end_time{};
	double frame_time{};
	double frame_time_mul_value = 1;
#endif

	// FWL error processing class
	FWL_MESSEGE f_messege;


public:


	
#ifdef USING_FRAME_TIME
	// set frame time mul value
	void set_frame_time_mul_value(double value) { frame_time_mul_value = value; }




	//reset frame time mul value to defalut
	void set_frame_time_mul_value_to_defalut() { frame_time_mul_value = 1; }




	// multiply movement with frame time
	double calc_ft(double movement, double additional_value = 1) { 
		return movement * frame_time * frame_time_mul_value * additional_value; 
	}

#endif



	// get current mode name
	std::string get_current_mode() { return current_mode; }




	void routine() {
		if (framework_initialization) {
#ifdef USING_FRAME_TIME
			start_time = clock();
#endif

			for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
				for (auto it = main_cont[i].begin(); it != main_cont[i].end();) {
					auto ptr = *it;
					if (ptr != nullptr) {
						if (!pause && !changing_mode) {
							ptr->update();
							ptr->check_collision();
						}
						ptr->render();
						ptr->check_delete();

						++it; 
					}

					else
						it = main_cont[i].erase(remove(main_cont[i].begin(), main_cont[i].end(), ptr));
				}
			}


#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER
			if (popup_initializtion) {
				for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
					for (auto it = popup_cont[i].begin(); it != popup_cont[i].end();) {
						auto ptr = *it;

						if (ptr != nullptr) {
							if (!changing_mode) {
								ptr->update();
								ptr->check_collision();
							}
							ptr->render();
							ptr->check_delete();

							++it;
						}

						else
							it = popup_cont[i].erase(remove(popup_cont[i].begin(), popup_cont[i].end(), ptr));
					}
				}
			}
#endif
#endif

#ifdef USING_FRAME_TIME
			end_time = clock();
			frame_time = (double)(end_time - start_time) / 1000;
#endif
		}
	}




	//init FWL
	void init(func startmode, std::string modename) {
		if (framework_initialization)
			f_messege.process_err("FWL init error::Reapeted initialization");

		framework_start = true;

		MODELIST m;
		mode_list = m.mode_list;

#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER
		popup_mode_list = m.popup_mode_list;
#endif 
#endif

		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			f_messege.process_err("FWL init error::Invalid mode", modename);
		f_messege.save_next_name(modename);


		startmode();
		current_mode = modename;
		f_messege.save_curr_name(modename);
		f_messege.process_FWL_init_messege();

		framework_initialization = true;
	}




	// change current mode
	void change_mode(func modefunc, std::string modename) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (current_mode == modename)
			f_messege.process_err("FWL mode error::Same mode as current mode");


		f_messege.save_prev_mode_name(current_mode);

		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			f_messege.process_err("FWL mode error::Invalid mode", modename);
		f_messege.save_next_name(modename);


		changing_mode = true;

#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER
		if (popup_initializtion)
			end_popup();
#endif
#endif

		modefunc();

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			sweep_layer(i);
			main_cont[i] = temp_cont[i];
			temp_cont[i].clear();
		}

		current_mode = modename;
		f_messege.save_curr_name(modename);
		f_messege.process_mode_change_messege();

		changing_mode = false;
	}




	// add object
	void add_object(FUNCTION*&& object, int layer) {
		if (!framework_start)
			f_messege.process_err("FWL init error::Invalid initialization");


		if (changing_mode) {
			if (layer >= NUMBER_OF_LAYER || layer < 0)
				f_messege.process_err("FWL object addition error::Layer out of bounds::temp container");

			temp_cont[layer].push_back(object);
		}

		else {
			if (layer >= NUMBER_OF_LAYER || layer < 0) {
				if(framework_initialization)
					f_messege.process_err("FWL object addition error::Layer out of bounds::main container");
				else
					f_messege.process_err("FWL init error::Layer out of bounds::main container");
			}

			main_cont[layer].push_back(object);
		}
	}




	// delete object
	void delete_object(FUNCTION* object, int layer) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			f_messege.process_err("FWL object deletion error::Layer out of bounds");


		auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), object);
		if (target != main_cont[layer].end()) {
			delete* target;
			*target = nullptr;
		}
		else 
			f_messege.process_err("FWL object deletion error::Can not find object");
	}




	// get ptr from other object
	FUNCTION* get_ptr(int layer, int index) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			f_messege.process_err("FWL ptr error::Layer out of bounds");


		if (index >= main_cont[layer].size())
			return nullptr;
		else
			return main_cont[layer][index];
	}




	// return number of objects of specific layer
	size_t get_layer_size(int layer) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			f_messege.process_err("FWL layer size error::Layer out of bounds");


		return main_cont[layer].size();
	}




	// delete objects of specific layer
	void sweep_layer(int layer) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			f_messege.process_err("FWL layer sweep error::Layer out of bounds");


		for (auto it = main_cont[layer].begin(); it != main_cont[layer].end();) {
			auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}




	// delete all object
	void sweep_all() {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");


		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = main_cont[i].begin(); it != main_cont[i].end();) {
				auto target = std::find(main_cont[i].begin(), main_cont[i].end(), *it);
				
				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}





#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER

	//init popup
	void init_popup(func modefunc, std::string modename, bool main_mode_pause_option = false) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Reapeted init");


		popup_mode_start = true;
		popup_mode_end = false;

		auto target = std::find(popup_mode_list.begin(), popup_mode_list.end(), modename);
		if (target == popup_mode_list.end())
			f_messege.process_popup_err("FWL popup init error::Invalid mode", modename);
		f_messege.save_next_popup_name(modename);

		pause = true;

		modefunc();
		prev_mode_name = current_mode; // save main mode name
		current_mode = modename;
		f_messege.save_curr_popup_name(modename);

		if (!main_mode_pause_option)  // stop main mode's update if pause option is true
			pause = false;

		f_messege.save_is_pause(pause);
		f_messege.process_popup_init_messege();

		popup_initializtion = true;
	}




	// change mode of popup
	void change_popup_mode(func modefunc, std::string modename) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");


		f_messege.save_prev_popup_mode_name(current_mode);

		auto target = std::find(popup_mode_list.begin(), popup_mode_list.end(), modename);
		if (target == popup_mode_list.end())
			f_messege.process_popup_err("FWL popup mode error::Invalid mode", modename);
		f_messege.save_next_popup_name(modename);


		changing_mode = true;

		modefunc();

		for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
			sweep_popup_layer(i);
			popup_cont[i] = temp_popup_cont[i];
			temp_popup_cont[i].clear();
		}

		current_mode = modename;
		f_messege.save_curr_popup_name(modename);
		f_messege.process_popup_mode_change_messege();

		changing_mode = false;
	}




	// end popup
	void end_popup() {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");


		popup_mode_end = true;

		sweep_popup_all();

		current_mode = prev_mode_name;
		f_messege.process_popup_close_messege();

		pause = false;
		popup_mode_start = false;
		popup_initializtion = false;
	}




	//add popup object
	void add_popup_object(POPUP_FUNCTION*&& object, int layer) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_mode_start)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");


		if (changing_mode) {
			if (layer >= NUMBER_OF_POPUP_LAYER || layer < 0)
				f_messege.process_popup_err("FWL popup object addition error::Layer out of bounds::temp container");

			temp_popup_cont[layer].push_back(object);
		}

		else {
			if (layer >= NUMBER_OF_POPUP_LAYER || layer < 0) {
				if (popup_initializtion)
					f_messege.process_popup_err("FWL popup object addition error::Layer out of bounds::main container");
				else
					f_messege.process_popup_err("FWL popup init error::Layer out of bounds::main container");
			}
			
			popup_cont[layer].push_back(object);
		}
	}




	// delete popup object
	void delete_popup_object(POPUP_FUNCTION* object, int layer) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");

		if (layer >= NUMBER_OF_POPUP_LAYER || layer < 0)
			f_messege.process_popup_err("FWL popup object deletion error::Layer out of bounds");

		
		auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), object);
		if (target != popup_cont[layer].end()) {
			delete* target;
			*target = nullptr;
		}

		else
			f_messege.process_popup_err("FWL popup object deletion error::Can not find object");
	}




	// return number of popup objects of specific popup layer
	size_t get_popup_layer_size(int layer) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");

		if (layer >= NUMBER_OF_POPUP_LAYER || layer < 0)
			f_messege.process_popup_err("FWL popup layer size error::Layer out of bounds");


		return popup_cont[layer].size();
	}




	// get ptr from other popup object
	POPUP_FUNCTION* get_popup_ptr(int layer, int index) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");

		if (layer >= NUMBER_OF_POPUP_LAYER || layer < 0)
			f_messege.process_popup_err("FWL popup ptr error::Layer out of bounds");

		
		if (index >= popup_cont[layer].size())
			return nullptr;
		else
			return popup_cont[layer][index];
	}




	// delete popup objects of specific popup layer
	void sweep_popup_layer(int layer) {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");
		
		if (layer >= NUMBER_OF_POPUP_LAYER || layer < 0)
			f_messege.process_popup_err("FWL popup layer sweep error::Layer out of bounds");


		for (auto it = popup_cont[layer].begin(); it != popup_cont[layer].end();) {
			auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}

		if(popup_mode_end)
			popup_cont[layer].clear();
	}




	// delete popup object all
	void sweep_popup_all() {
		if (!framework_initialization)
			f_messege.process_err("FWL init error::Invalid initialization");

		if (!popup_initializtion)
			f_messege.process_popup_err("FWL popup init error::Invalid initialization");


		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = popup_cont[i].begin(); it != popup_cont[i].end();) {
				auto target = std::find(popup_cont[i].begin(), popup_cont[i].end(), *it);

				delete* target;
				*target = nullptr;

				++it;
			}
		}

		if(popup_mode_end)
			for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) 
				popup_cont[i].clear();
	}

#endif
#endif

};

#endif
#endif

#endif