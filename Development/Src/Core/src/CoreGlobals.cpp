#include "CorePrivate.h"
#include "CoreManager.h"

namespace NGTech {
	CoreManager *core;

	/**
	*/
	CoreManager* GetCore(){
		return core;
	}

	/**
	*/
	void SetCore(CoreManager* _core){
		core = _core;
	}

	/**
	*/
	FileSystem* GetVFS(){
		return GetCore()->vfs;
	}

	/**
	*/
	I_Window* GetWindow(){
		return GetCore()->iWindow;
	}

	/**
	*/
	CVARManager* GetCvars(){
		return GetCore()->cvars;
	}

	/**
	*/
	I_Render* GetRender(){
		return GetCore()->iRender;
	}

	/**
	*/
	I_Audio* GetAudio(){
		return GetCore()->alSystem;
	}

	/**
	*/
	PhysSystem* GetPhysics(){
		return GetCore()->physSystem;
	}
	/**
	*/
	Cache* GetCache(){
		return GetCore()->cache;
	}

	/**
	*/
	GUI* GetGUI() {
		return GetCore()->gui;
	}

	/**
	*/
	Scene* GetScene(){
		return GetCore()->scene;
	}

	/**
	*/
	IGame* GetGame(){
		return GetCore()->game;
	}

	/**
	*/
	IScriptInterp* GetScripting(){
		return GetCore()->scripting;
	}

	/**
	*/
	CORE_API SkinnedMeshLoader* GetSkinnedMeshLoader(){
		return GetCore()->skinnedMeshLoader;
	}

	/**
	*/
	CORE_API MeshLoader* GetMeshLoader(){
		return GetCore()->meshLoader;
	}

	/**
	*/
#ifdef USE_STEAMWORKS
	SteamWorksMgr* GetSteamworksStats()	{
		return GetCore()->steamworks;
	}
#endif
}