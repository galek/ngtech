#pragma once

#include "coredll.h"

namespace NGTech {

	struct CORE_API CoreManager{
		CoreManager();

		virtual void quit()=0;
		virtual void initialise()=0;
		virtual void mainLoop()=0;
		virtual float GetLastFPS() = 0;
		virtual void LoadEngineModule(const char* _name){}
	public:
		bool running;
	public:
		struct IWindow*iWindow;
		struct I_Render*iRender;
		struct CVARManager*cvars;
		struct Log* log;
		class Config *config;
		class FileSystem*vfs;
		class IAudio *alSystem;
		class ILSystem *ilSystem;
		class PhysSystem *physSystem;

		class Cache *cache;
		class GUI*gui;
		class Scene*scene;
		struct IGame*game;
		struct IScriptInterp* scripting;
	};
}