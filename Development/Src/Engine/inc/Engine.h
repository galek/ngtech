#pragma once 

//***************************************************
#include "DLLDef.h"
//***************************************************
#include "..\..\Core\inc\CoreManager.h"
//***************************************************

namespace NGTech {
	struct IGame;
	class EnginePlugins;
	class EngineThreads;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main class. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API Engine : public CoreManager {
	public:
		Engine();

		virtual ~Engine();
		/**
		engines main loop
		*/
		void mainLoop();
		void editorLoop();

		void do_update();
		void do_render();
		void do_swap();

		/**
		exits the main loop
		*/
		void quit();
		void initialise(int _hwnd = 0);
		void setGame(IGame*_game);
		virtual float GetLastFPS();
		virtual float GetTimePerFrame();
		virtual void LoadEngineModule(const char* _name);
	private:
		void _preInit();
		void _setResources();
	private:
		EnginePlugins*plugins;
		EngineThreads*threads;
	};
	ENGINE_API Engine* GetEngine();
}