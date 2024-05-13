#ifndef FWL_MESSEGE_H
#define FWL_MESSEGE_H
#include <iostream>
#include <vector>
#include "FWL_config.h"

#ifdef USING_FWL
#if N_MAIN_LAYER


enum MainErrorTypes {
	INV_M_INIT,
	RPT_M_INIT,
	INV_M_MODE_IN_M_INIT,
	LOB_M_IN_M_INIT,
	INV_M_MODE_IN_SWITCH,
	SAME_M_MODE_IN_SWITCH,
	LOB_M_IN_SWITCH,
	INC_FUNC_IN_M_SWITCH,
	LOB_M_IN_ADD,
	LOB_M_IN_DELETE,
	UKN_M_IN_DELETE,
	LOB_M_IN_PTR,
	LOB_M_IN_FIND,
	LOB_M_IN_SIZE,
	LOB_M_IN_CLEAR_LYR
};


enum SubErrorTypes {
	INV_S_INIT,
	RPT_S_INIT,
	INV_S_MODE_IN_S_INIT,
	LOB_S_IN_S_INIT,
	INV_S_MODE_IN_SWITCH,
	SAME_S_MODE_IN_SWITCH,
	LOB_S_IN_SWITCH,
	INC_FUNC_IN_S_SWITCH,
	LOB_S_IN_ADD,
	LOB_S_IN_DELETE,
	UKN_S_IN_DELETE,
	LOB_S_IN_PTR,
	LOB_S_IN_FIND,
	LOB_S_IN_SIZE,
	LOB_S_IN_CLEAR_LYR
};


class FWL_MESSEGE {
private:
	std::vector<std::string>  MainModeList;
	std::string				  PrevMainModeName{};
	std::string				  CurrentMainModeName{};
	std::string				  NextMainModeName{};

	bool					  PauseState{};

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	std::vector<std::string> SubModeList; 
	std::string				 PrevSubModeName{};
	std::string				 CurrentSubModeName{};
	std::string				 NextSubModeName{};
#endif
#endif


public:
	void SV_NXT_M_MODE_NAME(std::string name) { NextMainModeName = name; }
	void SV_CURR_M_MODE_NAME(std::string name) { CurrentMainModeName = name; }
	void SV_PREV_M_MODE_NAME(std::string name) { PrevMainModeName = name; }

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	void SV_NXT_S_MODE_NAME(std::string name) { NextSubModeName = name; }
	void SV_CURR_S_MODE_NAME(std::string name) { CurrentSubModeName = name; }
	void SV_PREV_S_MODE_NAME(std::string name) { PrevSubModeName = name; }
#endif
#endif

	void SV_PAUSE_STATE(bool b) { PauseState = b; }

	void FWL_INIT_MSG() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "FWL main mode initialized" << std::endl << std::endl;
#endif
	}

	void SUB_INIT_MSG() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		if (PauseState)
			std::cout << "Main mode paused" << std::endl;
		std::cout << "Sub mode initialized" << std::endl << std::endl;
#endif
	}

	void SUB_END_MSG() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "Sub mode end" << std::endl << std::endl;
#endif
	}

	void MAIN_MODE_SWITCH_MSG() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "Main mode switched '" << PrevMainModeName << "' -> '" << CurrentMainModeName << "'." << std::endl << std::endl;
#endif
	}

	void SUB_MODE_SWITCH_MSG() {
#ifdef USING_DEBUG_MESSEGE
		std::cout << std::endl << "==========[MESSEGE]==========" << std::endl;
		std::cout << "Sub mode switched '" << PrevSubModeName << "' -> '" << CurrentSubModeName << "'." << std::endl << std::endl;
#endif
	}


	void MAIN_ERROR(int ERR_TYPE, std::string INV_MODE = "") {
		using namespace std;
#ifdef USING_DEBUG_MESSEGE
		cout << endl << "==========[ERROR OCCURED]==========" << endl << endl;

		switch (ERR_TYPE) {
		case INV_M_INIT:
			cout << "FWL main mode init error::Invalid FWL initialization" << endl;
			cout << "FWL main mode is not initialized yet." << endl;
			cout << "Execute FWL's function 'Init(func StartMode, std::string ModeName)' first." << endl;
			break;

		case RPT_M_INIT:
			cout << "FWL main mode init error::Reapeted init" << endl;
			cout << "Repeated FWL main mode initialization has been detected." << endl;
			break;

		case INV_M_MODE_IN_M_INIT:
			cout << "FWL main mode init error::Invalid main mode" << endl;
			cout << "Invalid main mode initialization has been detected." << endl << endl;
			cout << "Invalid main mode: '" << INV_MODE << "'" << endl << endl;
			cout << "[List of valid main modes]" << endl;

			for (auto& L : MainModeList)
				std::cout << "" << L << "" << std::endl;
			break;

		case LOB_M_IN_M_INIT:
			cout << "FWL main mode init error::Add objects outside of layer bounds" << endl;
			cout << "Addition of object out of layer bounds has been detected." << endl << endl;
			cout << "At main mode start: '" << NextMainModeName << "'" << endl;
			break;

		case INV_M_MODE_IN_SWITCH:
			cout << "FWL main mode switch error::Invalid main mode" << endl;
			cout << "Invalid main mode switch has been detected." << endl << endl;
			cout << "At main mode switch: '" << CurrentMainModeName << "' -> invalid main mode '" << INV_MODE << "'" << endl << endl;
			cout << "[List of valid main modes]" << endl;

			for (auto& L : MainModeList)
				cout << "" << L << "" << endl;
			break;

		case SAME_M_MODE_IN_SWITCH:
			cout << "FWL main mode switch error::Same main mode as current main mode" << endl;
			cout << "Switch to the same main mode as the current main mode has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case LOB_M_IN_SWITCH:
			cout << "FWL main mode switch error::Add objects outside of layer bounds" << endl;
			cout << "Addition of object out of layer bounds has been detected." << endl << endl;
			cout << "At main mode switch: '" << CurrentMainModeName << "' -> '" << NextMainModeName << "'" << endl;
			break;

		case INC_FUNC_IN_M_SWITCH:
			cout << "FWL main mode switch error::Incorrect function usage" << endl;
			cout << "Incorrect function use [AddSubObj()] has been detected." << endl << endl;
			cout << "At main mode switch: '" << CurrentMainModeName << "' -> '" << NextMainModeName << "'" << endl;
			break;

		case LOB_M_IN_ADD:
			cout << "FWL main mode object addition error::Layer out of bounds" << endl;
			cout << "Addition of object out of layer bounds has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case LOB_M_IN_DELETE:
			cout << "FWL main mode object deletion error::Layer out of bounds" << endl;
			cout << "Deletion of object out of layer bounds has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case UKN_M_IN_DELETE:
			cout << "FWL main mode object deletion error::Unknown object" << endl;
			cout << "Deletion of unknown object has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case LOB_M_IN_PTR:
			cout << "FWL main mode ptr error::Layer out of bounds" << endl;
			cout << "Getting ptr out of layer bounds has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case LOB_M_IN_FIND:
			cout << "FWL main mode find error::Layer out of bounds" << endl;
			cout << "Finding object out of layer bounds has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case LOB_M_IN_SIZE:
			cout << "FWL main mode layer size error::Layer out of bounds" << endl;
			cout << "Getting layer size out of layer bounds has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case LOB_M_IN_CLEAR_LYR:
			cout << "FWL main mode layer clear error::Layer out of bounds" << endl;
			cout << "Clearing layer out of layer bounds has been detected." << endl << endl;
			cout << "At main mode: '" << CurrentMainModeName << "'" << endl;
			break;
		}
#endif
		exit(1);
	}


	void SUB_ERROR(int ERR_TYPE, std::string INV_MODE = "") {
		using namespace std;
#ifdef USING_DEBUG_MESSEGE
		cout << endl << "==========[ERROR OCCURED]==========" << endl << endl;

		switch (ERR_TYPE) {
		case INV_S_INIT:
			cout << "FWL sub mode init error::Invalid Sub mode initialization" << endl;
			cout << "Sub mode is not initialized yet." << endl;
			cout << "Execute FWL's function 'InitSubMode(func StartModeFunc, std::string SubModeName, bool MainModePauseOption = 1)' first." << endl;
			break;

		case RPT_S_INIT:
			cout << "FWL sub mode init error::Reapeted init" << endl;
			cout << "Repeated Sub mode initialization has been detected." << endl;
			break;

		case INV_S_MODE_IN_S_INIT:
			cout << "FWL sub mode init error::Invalid sub mode" << endl;
			cout << "Invalid sub mode initialization has been detected." << endl << endl;
			cout << "Invalid sub mode: '" << INV_MODE << "'" << endl << endl;
			cout << "[List of valid sub modes]" << endl;

			for (auto& L : SubModeList)
				std::cout << "" << L << "" << std::endl;
			break;

		case LOB_S_IN_S_INIT:
			cout << "FWL sub mode init error::Add objects outside of layer bounds" << endl;
			cout << "Addition of object out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode start: '" << NextSubModeName << "'" << endl;
			break;

		case INV_S_MODE_IN_SWITCH:
			cout << "FWL sub mode mode switch error::Invalid sub mode" << endl;
			cout << "Invalid sub mode switch has been detected." << endl << endl;
			cout << "At sub mode switch: '" << CurrentSubModeName << "' -> invalid sub mode '" << INV_MODE << "'" << endl << endl;
			cout << "[List of valid sub modes]" << endl;

			for (auto& L : SubModeList)
				cout << "" << L << "" << endl;
			break;

		case SAME_S_MODE_IN_SWITCH:
			cout << "FWL sub mode switch error::Same main mode as current main mode" << endl;
			cout << "Switch to the same sub mode as the current sub mode has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentSubModeName << "'" << endl;
			break;

		case LOB_S_IN_SWITCH:
			cout << "FWL sub mode switch error::Add objects outside of layer bounds" << endl;
			cout << "Addition of object out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode switch: '" << CurrentSubModeName << "' -> '" << NextSubModeName << "'" << endl;
			break;

		case INC_FUNC_IN_S_SWITCH:
			cout << "FWL sub mode switch error::Incorrect function usage" << endl;
			cout << "Incorrect function use [AddMainObj()] has been detected." << endl << endl;
			cout << "At sub mode switch: '" << CurrentSubModeName << "' -> '" << NextSubModeName << "'" << endl;
			break;

		case LOB_S_IN_ADD:
			cout << "FWL sub mode object addition error::Layer out of bounds" << endl;
			cout << "Addition of object out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentSubModeName << "'" << endl;
			break;

		case LOB_S_IN_DELETE:
			cout << "FWL sub mode object deletion error::Layer out of bounds" << endl;
			cout << "Deletion of object out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentSubModeName << "'" << endl;
			break;

		case UKN_S_IN_DELETE:
			cout << "FWL sub mode object deletion error::Unknown object" << endl;
			cout << "Deletion of unknown object has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentSubModeName << "'" << endl;
			break;

		case LOB_S_IN_PTR:
			cout << "FWL sub mode ptr error::Layer out of bounds" << endl;
			cout << "Getting ptr out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentSubModeName << "'" << endl;
			break;

		case LOB_S_IN_FIND:
			cout << "FWL sub mode find error::Layer out of bounds" << endl;
			cout << "Finding object out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentMainModeName << "'" << endl;
			break;

		case LOB_S_IN_SIZE:
			cout << "FWL sub mode layer size error::Layer out of bounds" << endl;
			cout << "Getting layer size out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentSubModeName << "'" << endl;
			break;

		case LOB_S_IN_CLEAR_LYR:
			cout << "FWL sub mode layer clear error::Layer out of bounds" << endl;
			cout << "Clearing layer out of layer bounds has been detected." << endl << endl;
			cout << "At sub mode: '" << CurrentSubModeName << "'" << endl;
			break;
		}
#endif
		exit(1);
	}


	FWL_MESSEGE() {
		MODELIST M;

		MainModeList = M.MainModeList;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		SubModeList = M.SubModeList;
#endif
#endif
	}
};

#endif
#endif

#endif
