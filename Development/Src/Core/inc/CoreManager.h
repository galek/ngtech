#pragma once

#include "../../Common/Types.h"
#include "coredll.h"

namespace NGTech {
	/**
	*/
	struct IWindow;
	struct I_Render;
	struct CVARManager;
	struct Log;
	class Config;
	class FileSystem;
	class I_Audio;
	class PhysSystem;
	class Cache;
	class GUI;
	class Scene;
	struct IGame;
	struct IScriptInterp;
	class SystemInfo;
	/**
	*/
	struct CORE_API CoreManager{
		CoreManager();
		~CoreManager();

		virtual void quit() = 0;
		virtual void initialise(int _hwnd) = 0;
		virtual void mainLoop() = 0;
		virtual float GetLastFPS() = 0;
		virtual void LoadEngineModule(const char* _name){}
		ENGINE_INLINE void SetRender(I_Render*_r){ iRender = _r; }
		ENGINE_INLINE bool isEditor(){ return mIsEditor; }
		ENGINE_INLINE void RunEditor(bool _ed){ mIsEditor = _ed; }
	public:
		bool running;
	public:
		IWindow*iWindow;
		I_Render*iRender;
		CVARManager*cvars;
		Log* log;
		Config *config;
		FileSystem*vfs;
		I_Audio *alSystem;
		PhysSystem *physSystem;
		SystemInfo *info;

		Cache *cache;
		GUI*gui;
		Scene*scene;
		IGame*game;
		IScriptInterp* scripting;
		bool mIsEditor;
	};
	// For Using in Render and another libs,what not will link with Engine.dll
	// After creation Engine pointer on Core will pointer on Engine
	CORE_API CoreManager* GetCore();
	void CORE_API SetCore(CoreManager* _core);
	CORE_API FileSystem* GetVFS();
	CORE_API IWindow* GetWindow();
	CORE_API CVARManager* GetCvars();
	CORE_API I_Render* GetRender();
	CORE_API I_Audio* GetAudio();
	CORE_API PhysSystem* GetPhysics();
	CORE_API Cache* GetCache();
	CORE_API GUI* GetGUI();
	CORE_API Scene* GetScene();
	CORE_API IGame* GetGame();
	CORE_API IScriptInterp* GetScripting();
}