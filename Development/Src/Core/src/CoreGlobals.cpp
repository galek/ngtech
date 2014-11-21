#include "CorePrivate.h"
#include "CoreManager.h"

namespace NGTech {
	static CoreManager *core;

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
	FileSystem* GetVFS() {
		if (core&&core->vfs) {
			static auto ptr = GetCore()->vfs;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	I_Window* GetWindow() {
		if (core&&core->iWindow) {
			static auto ptr = GetCore()->iWindow;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	CVARManager* GetCvars() {
		if (core&&core->cvars) {
			static auto ptr = GetCore()->cvars;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	I_Render* GetRender() {
		if (core&&core->iRender) {
			static auto ptr = GetCore()->iRender;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	I_Audio* GetAudio() {
		if (core&&core->alSystem) {
			static auto ptr = GetCore()->alSystem;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	PhysSystem* GetPhysics() {
		if (core&&core->physSystem) {
			static auto ptr = GetCore()->physSystem;
			return ptr;
		}
		return nullptr;
	}
	/**
	*/
	Cache* GetCache() {
		if (core&&core->cache) {
			static auto ptr = GetCore()->cache;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	GUI* GetGUI() {
		if (core&&core->gui) {
			static auto ptr = GetCore()->gui;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	Scene* GetScene() {
		if (core&&core->scene) {
			static auto ptr = GetCore()->scene;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	IGame* GetGame() {
		if (core&&core->game) {
			static auto ptr = GetCore()->game;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	IScriptInterp* GetScripting() {
		if (core&&core->scripting) {
			static auto ptr = GetCore()->scripting;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	CORE_API SkinnedMeshLoader* GetSkinnedMeshLoader() {
		if (core&&core->skinnedMeshLoader) {
			static auto ptr = GetCore()->skinnedMeshLoader;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
	CORE_API MeshLoader* GetMeshLoader() {
		if (core&&core->meshLoader) {
			static auto ptr = GetCore()->meshLoader;
			return ptr;
		}
		return nullptr;
	}

	/**
	*/
#ifdef USE_STEAMWORKS
	SteamWorksMgr* GetSteamworksStats()	{
		if (core&&core->steamworks) {
			static auto ptr = GetCore()->steamworks;
			return ptr;
		}
		return nullptr;
}
#endif
}