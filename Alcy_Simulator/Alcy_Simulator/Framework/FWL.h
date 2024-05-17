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
#if N_MAIN_LAYER

class FWL {
private:
	std::array<std::deque<MAIN_CLS*>, N_MAIN_LAYER> MainCont{};
	std::array<std::deque<MAIN_CLS*>, N_MAIN_LAYER> MainTempCont{};

	std::vector<std::string> MainModeList;

	std::string              PrevModeName{};
	std::string              CurrentModeName{};
	std::string              CurrentMainModeName{};

	bool                     InStartMainMode{};
	bool					 MainModeInitState{};


#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	std::array<std::deque<SUB_CLS*>, N_SUB_LAYER> SubCont{};
	std::array<std::deque<SUB_CLS*>, N_SUB_LAYER> SubTempCont{};

	std::vector<std::string> SubModeList;

	std::string              CurrentSubModeName{};

	bool					 InStartSubMode{};
	bool					 SubModeInitState{};
	bool					 InEndSubMode{};
#endif
#endif

	bool					 ModeSwitchState{};
	bool					 PauseState{};

	// for debug messeges
	bool					 DBG_INIT_SUB_MODE{};
	bool					 DBG_SWITCH_MAIN_MODE{};
	bool					 DBG_SWITCH_SUB_MODE{};


// frame time
#ifdef USING_FRAME_TIME
	clock_t					 StartTime{}, EndTime{};
	double					 FrameTime{};
	double					 FrameTimeMulValue = 1;
#endif

	typedef void (*func)(void);

	// FWL error processing class
	FWL_MESSEGE				 F_Messege;


public:

#ifdef USING_FRAME_TIME
	// set frame time mul value
	void SetFrameTimeMul(double value) { FrameTimeMulValue = value; }

	//reset frame time mul value to defalut
	void ResetFrameTimeMul() { FrameTimeMulValue = 1; }

#ifdef USING_FRAME_TIME_OUTSIDE
	// input frame time from outside
	void InputFrameTime(double time) { FrameTime = time; }
#endif

	// multiply frame time
	double FT(double movement, double additional_value = 1) { 
		return movement * FrameTime * FrameTimeMulValue * additional_value; 
	}

#endif

	// get current mode name
	std::string CurrentMode() const { return CurrentModeName; }

	// get current main mode name
	std::string CurrentMainMode() const { return CurrentMainModeName; }

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	// get current sub mode name
	std::string CurrentSubMode() const { return CurrentSubModeName; }
#endif
#endif

	void Routine() {
		if (MainModeInitState) {
#ifdef USING_FRAME_TIME
#ifndef USING_FRAME_TIME_OUTSIDE
			StartTime = clock();
#endif
#endif
			for (int i = 0; i < N_MAIN_LAYER; ++i) {
				for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
					auto Ptr = *It;
					if (Ptr != nullptr) {
						if (!PauseState && !ModeSwitchState) {
							Ptr->Update();
							Ptr->CheckCollision();
						}
						Ptr->Render();
						Ptr->CheckDelete();

						++It; 
					}

					else
						It = MainCont[i].erase(remove(MainCont[i].begin(), MainCont[i].end(), Ptr));
				}
			}

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
			if (SubModeInitState) {
				for (int i = 0; i < N_SUB_LAYER; ++i) {
					for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
						auto Ptr = *It;
						if (Ptr != nullptr) {
							if (!ModeSwitchState) {
								Ptr->Update();
								Ptr->CheckCollision();
							}
							Ptr->Render();
							Ptr->CheckDelete();

							++It;
						}

						else
							It = SubCont[i].erase(remove(SubCont[i].begin(), SubCont[i].end(), Ptr));
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




	//init FWL
	void InitMainMode(func MainModeFunc, std::string MainModeName) {
		if (MainModeInitState)
			F_Messege.MAIN_ERROR(RPT_M_INIT);

		InStartMainMode = true;

		MODELIST M;
		MainModeList = M.MainModeList;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		SubModeList = M.SubModeList;
#endif 
#endif

		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end())
			F_Messege.MAIN_ERROR(INV_M_MODE_IN_M_INIT, MainModeName);

		F_Messege.SV_NXT_M_MODE_NAME(MainModeName);

		MainModeFunc();

		CurrentModeName = MainModeName;
		CurrentMainModeName = MainModeName;

		F_Messege.SV_CURR_M_MODE_NAME(MainModeName);
		F_Messege.FWL_INIT_MSG();

		MainModeInitState = true;
	}




	// change current mode
	void SwitchMainMode(func MainModeFunc, std::string MainModeName) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (CurrentModeName == MainModeName)
			F_Messege.MAIN_ERROR(SAME_M_MODE_IN_SWITCH);


		F_Messege.SV_PREV_M_MODE_NAME(CurrentModeName);

		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end())
			F_Messege.MAIN_ERROR(INV_M_MODE_IN_SWITCH, MainModeName);

		F_Messege.SV_NXT_M_MODE_NAME(MainModeName);

		ModeSwitchState = true;

		// debug value
		DBG_SWITCH_MAIN_MODE = true;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		if (SubModeInitState)
			EndSubMode();
#endif
#endif

		MainModeFunc();

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			ClearMainLayer(i);
			MainCont[i] = MainTempCont[i];
			MainTempCont[i].clear();
		}

		CurrentModeName = MainModeName;
		CurrentMainModeName = MainModeName;

		F_Messege.SV_CURR_M_MODE_NAME(MainModeName);
		F_Messege.MAIN_MODE_SWITCH_MSG();

		// debug value
		DBG_SWITCH_MAIN_MODE = false;

		ModeSwitchState = false;
	}




	// add object
	void AddMainObj(MAIN_CLS*&& Object, int Layer) {
		if (!InStartMainMode)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (DBG_SWITCH_SUB_MODE || DBG_INIT_SUB_MODE)
			F_Messege.SUB_ERROR(INC_FUNC_IN_S_SWITCH);


		if (ModeSwitchState) {
			if (Layer >= N_MAIN_LAYER || Layer < 0)
				F_Messege.MAIN_ERROR(LOB_M_IN_SWITCH);

			MainTempCont[Layer].push_back(Object);
		}

		else {
			if (Layer >= N_MAIN_LAYER || Layer < 0) {
				if(MainModeInitState)
					F_Messege.MAIN_ERROR(LOB_M_IN_ADD);
				else
					F_Messege.MAIN_ERROR(LOB_M_IN_M_INIT);
			}

			MainCont[Layer].push_back(Object);
		}
	}




	// delete object
	void DeleteMainObj(MAIN_CLS* Object, int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_DELETE);


		auto Target = std::find(MainCont[Layer].begin(), MainCont[Layer].end(), Object);
		if (Target != MainCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}
		else 
			F_Messege.MAIN_ERROR(UKN_M_IN_DELETE);
	}




	// delete main object outside of object class
	void DeleteMainObj_Layer_Single(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_DELETE);


		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = MainObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag) {
				Target->ActivateDeleteFlag(true);
				break;
			}
		}
	}




	void DeleteMainObj_Layer_All(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_DELETE);


		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = MainObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag)
				Target->ActivateDeleteFlag(true);
		}
	}




	void DeleteMainObj_Entire_Single(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		bool ObjFind{};

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = MainObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag) {
					Target->ActivateDeleteFlag(true);
					ObjFind = true;
					break;
				}
			}
		}
	}




	void DeleteMainObj_Entire_All(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = MainObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag)
					Target->ActivateDeleteFlag(true);
			}
		}
	}




	void DeleteMainObj_Layer_Index(int Layer, int Index, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_FIND);


		auto Target = MainObjPtr(Layer, Index);

		if (Target != nullptr && Target->GetTag() == Tag)
			Target->ActivateDeleteFlag(true);
	}




	// return number of objects of specific layer
	size_t MainLayerSize(int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_SIZE);


		return MainCont[Layer].size();
	}




	// find single object ptr on layer
	MAIN_CLS* FindMainObj_Layer_Single(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_FIND);


		MAIN_CLS* Obj{};
		bool ObjFind{};

		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = MainObjPtr(Layer, i);

			if (Ptr != nullptr && Ptr->GetTag() == Tag) {
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




	// find single object on entire container
	MAIN_CLS* FindMainObj_Entire_Single(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);


		MAIN_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = MainObjPtr(i, j);
				if (Ptr != nullptr && Ptr->GetTag() == Tag) {
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




	// find many objects ptr on layer, use with for()
	MAIN_CLS* FindMainObj_Layer_Index(int Layer, int Index, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (Layer >= N_MAIN_LAYER || Layer < 0 )
			F_Messege.MAIN_ERROR(LOB_M_IN_FIND);


		auto Ptr = MainObjPtr(Layer, Index);

		if (Ptr != nullptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}


#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	//init sub mode
	void InitSubMode(func SubModeFunc, std::string SubModeName, bool MainModePauseOption = false) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (SubModeInitState)
			F_Messege.SUB_ERROR(RPT_S_INIT);


		InStartSubMode = true;
		InEndSubMode = false;

		// debug value
		DBG_INIT_SUB_MODE = true;

		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end())
			F_Messege.SUB_ERROR(INV_S_MODE_IN_S_INIT, SubModeName);

		F_Messege.SV_NXT_S_MODE_NAME(SubModeName);

		PauseState = true;

		SubModeFunc();

		PrevModeName = CurrentModeName; // save main mode name
		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;

		F_Messege.SV_CURR_S_MODE_NAME(SubModeName);

		if (!MainModePauseOption)  // stop main mode's update if pause option is true
			PauseState = false;

		F_Messege.SV_PAUSE_STATE(PauseState);
		F_Messege.SUB_INIT_MSG();

		// debug value
		DBG_INIT_SUB_MODE = false;

		SubModeInitState = true;
	}




	// change mode of popup
	void SwitchSubMode(func SubModeFunc, std::string SubModeName) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);


		F_Messege.SV_PREV_S_MODE_NAME(CurrentModeName);

		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end())
			F_Messege.SUB_ERROR(INV_S_MODE_IN_SWITCH, SubModeName);

		F_Messege.SV_NXT_S_MODE_NAME(SubModeName);

		ModeSwitchState = true;

		// debug value
		DBG_SWITCH_SUB_MODE = true;

		SubModeFunc();

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			ClearSubLayer(i);
			SubCont[i] = SubTempCont[i];
			SubTempCont[i].clear();
		}

		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;

		F_Messege.SV_CURR_S_MODE_NAME(SubModeName);
		F_Messege.SUB_MODE_SWITCH_MSG();

		// debug value
		DBG_SWITCH_SUB_MODE = false;

		ModeSwitchState = false;
	}




	// end popup mode
	void EndSubMode() {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);


		InEndSubMode = true;

		ClearSubAll();

		CurrentModeName = PrevModeName;
		CurrentSubModeName = "";

		F_Messege.SUB_END_MSG();

		PauseState = false;
		InStartSubMode = false;
		SubModeInitState = false;
	}




	//add popup object
	void AddSubObj(SUB_CLS*&& Object, int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (DBG_SWITCH_MAIN_MODE)
			F_Messege.MAIN_ERROR(INC_FUNC_IN_M_SWITCH);

		if (!InStartSubMode)
			F_Messege.SUB_ERROR(INV_S_INIT);


		if (ModeSwitchState) {
			if (Layer >= N_SUB_LAYER || Layer < 0)
				F_Messege.SUB_ERROR(LOB_S_IN_SWITCH);

			SubTempCont[Layer].push_back(Object);
		}

		else {
			if (Layer >= N_SUB_LAYER || Layer < 0) {
				if (SubModeInitState)
					F_Messege.SUB_ERROR(LOB_S_IN_ADD);
				else
					F_Messege.SUB_ERROR(LOB_S_IN_S_INIT);
			}
			
			SubCont[Layer].push_back(Object);
		}
	}




	// delete popup object
	void DeleteSubObj(SUB_CLS* Object, int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_DELETE);

		
		auto Target = std::find(SubCont[Layer].begin(), SubCont[Layer].end(), Object);
		if (Target != SubCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}

		else
			F_Messege.SUB_ERROR(UKN_S_IN_DELETE);
	}




	// delete main object outside of object class
	void DeleteSubObj_Layer_Single(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_DELETE);


		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = SubObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag) {
				Target->ActivateDeleteFlag(true);
				break;
			}
		}
	}




	void DeleteSubObj_Layer_All(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_DELETE);


		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = SubObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag)
				Target->ActivateDeleteFlag(true);
		}
	}




	void DeleteSubObj_Entire_Single(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);


		bool ObjFind{};

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = SubObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag) {
					Target->ActivateDeleteFlag(true);
					ObjFind = true;
					break;
				}
			}
		}
	}





	void DeleteSubObj_Entire_All(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);


		for (int i = 0; i < N_SUB_LAYER; ++i) {
			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = SubObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag)
					Target->ActivateDeleteFlag(true);
			}
		}
	}




	void DeleteSubObj_Layer_Index(int Layer, int Index, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_DELETE);


		auto Target = SubObjPtr(Layer, Index);

		if (Target != nullptr && Target->GetTag() == Tag)
			Target->ActivateDeleteFlag(true);
	}




	// return number of popup objects of specific popup layer
	size_t SubLayerSize(int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_SIZE);


		return SubCont[Layer].size();
	}




	// find popup object ptr on layer
	SUB_CLS* FindSubObj_Layer_Single(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_FIND);


		SUB_CLS* Obj{};
		bool ObjFind{};

		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = SubObjPtr(Layer, i);

			if (Ptr != nullptr && Ptr->GetTag() == Tag) {
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




	// find popup object on entire container
	SUB_CLS* FindSubObj_Entire_Single(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);


		SUB_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = SubObjPtr(i, j);
				if (Ptr != nullptr && Ptr->GetTag() == Tag) {
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




	// find many popup object ptr on layer, use with for()
	SUB_CLS* FindSubObj_Layer_Index(int Layer, int Index, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);

		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_FIND);


		auto Ptr = SubObjPtr(Layer, Index);

		if (Ptr != nullptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}

#endif
#endif


private:
	// get obj ptr from other object
	MAIN_CLS* MainObjPtr(int Layer, int Index) {
		if (Index >= MainCont[Layer].size())
			return nullptr;
		else
			return MainCont[Layer][Index];
	}




	// delete objects of specific layer
	void ClearMainLayer(int Layer) {
		for (auto It = MainCont[Layer].begin(); It != MainCont[Layer].end();) {
			auto Target = std::find(MainCont[Layer].begin(), MainCont[Layer].end(), *It);

			delete* Target;
			*Target = nullptr;

			++It;
		}
	}




	// delete all object
	void ClearMainAll() {
		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
				auto Target = std::find(MainCont[i].begin(), MainCont[i].end(), *It);

				delete* Target;
				*Target = nullptr;

				++It;
			}
		}
	}



#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// get ptr from other popup object
	SUB_CLS* SubObjPtr(int Layer, int Index) {
		if (Index >= SubCont[Layer].size())
			return nullptr;
		else
			return SubCont[Layer][Index];
	}




	// delete popup objects of specific popup layer
	void ClearSubLayer(int Layer) {
		for (auto It = SubCont[Layer].begin(); It != SubCont[Layer].end();) {
			auto Target = std::find(SubCont[Layer].begin(), SubCont[Layer].end(), *It);

			delete* Target;
			*Target = nullptr;

			++It;
		}

		if (InEndSubMode)
			SubCont[Layer].clear();
	}




	// delete popup object all
	void ClearSubAll() {
		for (int i = 0; i < N_SUB_LAYER; ++i) {
			for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
				auto Target = std::find(SubCont[i].begin(), SubCont[i].end(), *It);

				delete* Target;
				*Target = nullptr;

				++It;
			}
		}

		if (InEndSubMode)
			for (int i = 0; i < N_SUB_LAYER; ++i)
				SubCont[i].clear();
	}

#endif
#endif

};

#endif
#endif

#endif