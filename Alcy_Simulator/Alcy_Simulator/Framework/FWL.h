// This FWL is a lightweight version with all error check codes removed.
// Please be careful when using it as there is no error check code.
// Not recommended unless you are creating a performance oriented program.

#ifndef FWL_H
#define FWL_H
#pragma warning(disable: 28020)

#include <iostream>
#include <algorithm>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

#include "FWL_config.h"

#ifdef USING_FWL
#if N_MAIN_LAYER

class FWL {
private:
	std::array<std::deque<MAIN_CLS*>, N_MAIN_LAYER> MainCont{};
	std::vector<std::string> MainModeList;

	std::string              PrevModeName{};
	std::string              CurrentModeName{};
	std::string              CurrentMainModeName{};

	bool					 MainModeInitState{};


#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	std::array<std::deque<SUB_CLS*>, N_SUB_LAYER> SubCont{};
	std::vector<std::string> SubModeList;

	std::string              CurrentSubModeName{};

	bool					 SubModeInitState{};
#endif
#endif

	bool					 ModeSwitchState{};
	bool					 PauseState{};


// frame time
#ifdef USING_FRAME_TIME
	clock_t					 StartTime{}, EndTime{};
	double					 FrameTime{};
	double					 FrameTimeMulValue = 1;
#endif

	// callable function type
	typedef void (*func)(void);


public:

#ifdef USING_FRAME_TIME
	// Sets the frame-time multiple
	void SetFrameTimeMul(double value) { FrameTimeMulValue = value; }

	// Sets the framerate multiple to its initial setting (x1)
	void ResetFrameTimeMul() { FrameTimeMulValue = 1; }

#ifdef USING_FRAME_TIME_OUTSIDE
	// Enter the frame time from the outside
	void InputFrameTime(double time) { FrameTime = time; }
#endif

	// Multiply the frame time
	double FT(double movement, double additional_value = 1) { 
		return movement * FrameTime * FrameTimeMulValue * additional_value; 
	}

#endif

	// Returns the name of the currently running mode
	std::string CurrentMode() { return CurrentModeName; }

	// Returns the name of the main mode currently running
	std::string CurrentMainMode() { return CurrentMainModeName; }

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	// Returns the name of the sub mode currently running
	std::string CurrentSubMode() { return CurrentSubModeName; }
#endif
#endif

	// Runs a framework routine
	void Routine() {
		if (MainModeInitState) {
#ifdef USING_FRAME_TIME
#ifndef USING_FRAME_TIME_OUTSIDE
			StartTime = clock();
#endif
#endif
			// Main mode routine
			for (int i = 0; i < N_MAIN_LAYER; ++i) {
				for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
					auto Ptr = *It;

					// If the object is not nullptr, update and output the object
					if (Ptr) {
						if (!PauseState && !ModeSwitchState) {
							Ptr->Update();
							Ptr->CheckCollision();
						}
						Ptr->Render();
					}

					// Verify that the updated object is nullptr, and if it is nullptr, go to the else statement
					if(Ptr)
						++It; 

					// If the object is nullptr, delete the pointer that pointed to it from the container
					else
						It = MainCont[i].erase(It);
				}
			}

#ifdef USING_SUB_MODE
#if N_SUB_LAYER

			// Sub mode routine
			if (SubModeInitState) {
				for (int i = 0; i < N_SUB_LAYER; ++i) {
					for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
						auto Ptr = *It;

						// If the object is not nullptr, update and output the object
						if (Ptr) {
							if (!ModeSwitchState) {
								Ptr->Update();
								Ptr->CheckCollision();
							}
							Ptr->Render();
						}

						// Verify that the updated object is nullptr, and if it is nullptr, go to the else statement
						if (Ptr)
							++It;

						// If the object is nullptr, delete the pointer that pointed to it from the container
						else
							It = SubCont[i].erase(It);
					}
				}
			}
#endif
#endif

#ifdef USING_FRAME_TIME
#ifndef USING_FRAME_TIME_OUTSIDE
			EndTime = clock();
			FrameTime = (double)(EndTime - StartTime) / 1000;
#endif
#endif
		}
	}




	// Initializes the main mode
	void InitMainMode(func MainModeFunc, std::string MainModeName) {
		// Loads a list of modes created in FWL_config into the framework
		MODELIST M;
		MainModeList = M.MainModeList;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		SubModeList = M.SubModeList;
#endif 
#endif
		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end()) {
			std::cout << "Can not find main mode." << std::endl;
			exit(1);
		}

		// Run the mode start function
		MainModeFunc();

		// If you successfully ran the mode start function, specify the mode name for the current mode and current main mode
		CurrentModeName = MainModeName;
		CurrentMainModeName = MainModeName;

		// Enable Main Mode Routine
		MainModeInitState = true;
	}




	// Switch the currently running main mode to another main mode
	void SwitchMainMode(func MainModeFunc, std::string MainModeName) {
		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end()) {
			std::cout << "Can not find main mode." << std::endl;
			exit(1);
		}

		// When the mode switch state is activated, all object updates in the main mode and submode are interrupted
		ModeSwitchState = true;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		if (SubModeInitState)
			EndSubMode();
#endif
#endif

		// Delete all objects in the main mode container
		ClearMainAll();

		// Run the mode start function
		MainModeFunc();

		// If the mode start function has been successfully executed, change the name of the current mode and current main mode
		CurrentModeName = MainModeName;
		CurrentMainModeName = MainModeName;

		// The mode transition state is disabled and the main mode and submode object update resumes
		ModeSwitchState = false;
	}




	// Adds an object to a specific main mode layer
	void AddMainObj(MAIN_CLS*&& Object, int Layer) {
		MainCont[Layer].push_back(Object);
	}




	// Delete an object from the main mode container, use within the object class
	void DeleteMainObj(MAIN_CLS* Object, int Layer) {
		auto Target = std::find(MainCont[Layer].begin(), MainCont[Layer].end(), Object);
		if (Target != MainCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}
	}




	// Returns the number of objects in a specific main mode layer
	size_t MainLayerSize(int Layer) {
		return MainCont[Layer].size();
	}




	// Locate the pointer to a single object in the specific main mode layer
	MAIN_CLS* FindMainObj_Layer_Single(int Layer, std::string Tag) {
		MAIN_CLS* Obj{};
		bool ObjFind{};

		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = MainObjPtr(Layer, i);

			if (Ptr && Ptr->GetTag() == Tag) {
				Obj = Ptr;
				ObjFind = true;
				break;
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// Locate the pointer to a single object in the main mode container
	MAIN_CLS* FindMainObj_Entire_Single(std::string Tag) {
		MAIN_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = MainObjPtr(i, j);
				if (Ptr && Ptr->GetTag() == Tag) {
					Obj = Ptr;
					ObjFind = true;
					break;
				}
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// Locate multiple objects in the specific main mode layer, use with 'for()'
	MAIN_CLS* FindMainObj_Layer_Index(int Layer, int Index, std::string Tag) {
		auto Ptr = MainObjPtr(Layer, Index);

		if (Ptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}


#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// Initializes the submode
	void InitSubMode(func SubModeFunc, std::string SubModeName, bool MainModePauseOption = false) {
		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end()) {
			std::cout << "Can not find sub mode." << std::endl;
			exit(1);
		}

		PauseState = true;

		// Run the mode start function
		SubModeFunc();

		// If the mode start function has been successfully executed, 
		// save the main mode that was last executed and specify the current mode and the current sub mode
		PrevModeName = CurrentModeName;
		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;

		// If MainModePauseOption is set to true, pause the main mode update
		if (!MainModePauseOption)
			PauseState = false;

		// Enable Sub Mode Routine
		SubModeInitState = true;
	}




	// Switch the currently running sub mode to another sub mode
	void SwitchSubMode(func SubModeFunc, std::string SubModeName) {
		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end()) {
			std::cout << "Can not find sub mode." << std::endl;
			exit(1);
		}

		// When the mode switch state is activated, all object updates in the main mode and submode are interrupted
		ModeSwitchState = true;

		// Delete all objects in the sub mode container
		ClearSubAll();

		// Run the mode start function
		SubModeFunc();

		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;

		// The mode transition state is disabled and the main mode and submode object update resumes
		ModeSwitchState = false;
	}




	// Delete all objects in the sub mode container and exit the submode
	void EndSubMode() {
		// Delete all objects in the sub mode container
		ClearSubAll();

		// Change the current mode to the last main mode you ran, and delete the current submode
		CurrentModeName = PrevModeName;
		CurrentSubModeName = "";

		// The main mode pause is released, and the remaining submode settings are initialized
		PauseState = false;
		SubModeInitState = false;
	}




	// Adds an object to a specific sub mode layer
	void AddSubObj(SUB_CLS*&& Object, int Layer) {
		SubCont[Layer].push_back(Object);
	}




	// Delete an object from the sub mode container, use within the object class
	void DeleteSubObj(SUB_CLS* Object, int Layer) {
		auto Target = std::find(SubCont[Layer].begin(), SubCont[Layer].end(), Object);
		if (Target != SubCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}
	}




	// Returns the number of objects in a specific sub mode layer
	size_t SubLayerSize(int Layer) {
		return SubCont[Layer].size();
	}




	// Locate the pointer to a single object in the specific sub mode layer
	SUB_CLS* FindSubObj_Layer_Single(int Layer, std::string Tag) {
		SUB_CLS* Obj{};
		bool ObjFind{};

		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = SubObjPtr(Layer, i);

			if (Ptr && Ptr->GetTag() == Tag) {
				Obj = Ptr;
				ObjFind = true;
				break;
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// Locate the pointer to a single object in the sub mode container
	SUB_CLS* FindSubObj_Entire_Single(std::string Tag) {
		SUB_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = SubObjPtr(i, j);
				if (Ptr && Ptr->GetTag() == Tag) {
					Obj = Ptr;
					ObjFind = true;
					break;
				}
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// Locate multiple objects in the specific sub mode layer, use with 'for()'
	SUB_CLS* FindSubObj_Layer_Index(int Layer, int Index, std::string Tag) {
		auto Ptr = SubObjPtr(Layer, Index);

		if (Ptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}

#endif
#endif


private:
	// Locate the object pointer through a specific index of a specific main mode layer
	MAIN_CLS* MainObjPtr(int Layer, int Index) {
		if (Index >= MainCont[Layer].size())
			return nullptr;
		else
			return MainCont[Layer][Index];
	}




	// Delete all objects that exist in a specific main mode layer
	void ClearMainLayer(int Layer) {
		for (auto It = MainCont[Layer].begin(); It != MainCont[Layer].end();) {
			delete* It;
			*It = nullptr;

			It = MainCont[Layer].erase(It);
		}
	}




	// Delete all objects that exist in the main mode container
	void ClearMainAll() {
		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
				delete* It;
				*It = nullptr;

				It = MainCont[i].erase(It);
			}
		}
	}



#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// Locate the object pointer through a specific index of a specific sub mode layer
	SUB_CLS* SubObjPtr(int Layer, int Index) {
		if (Index >= SubCont[Layer].size())
			return nullptr;
		else
			return SubCont[Layer][Index];
	}




	// Delete all objects that exist in a specific sub mode layer
	void ClearSubLayer(int Layer) {
		for (auto It = SubCont[Layer].begin(); It != SubCont[Layer].end();) {
			delete* It;
			*It = nullptr;

			It = SubCont[Layer].erase(It);
		}
	}




	// Delete all objects that exist in the sub mode container
	void ClearSubAll() {
		for (int i = 0; i < N_SUB_LAYER; ++i) {
			for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
				delete* It;
				*It = nullptr;

				It = SubCont[i].erase(It);
			}
		}
	}

#endif
#endif

};

#endif
#endif

#endif