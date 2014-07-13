#pragma once 

//***************************************************
#include "DLLDef.h"
//***************************************************
#include "..\..\Core\CoreManager.h"
//***************************************************

namespace NGTech {
	struct IGame;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main class. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API Engine : public CoreManager {
	public:
		Engine();

		~Engine();
		/**
		engines main loop
		*/
		void mainLoop();
		/**
		exits the main loop
		*/
		void quit();
		void initialise();
		void setGame(IGame*_game);
	private:
		void _preInit();
		void _setResources();	
	};
	ENGINE_API Engine* GetEngine();
	ENGINE_API FileSystem* GetVFS();
	ENGINE_API IWindow* GetWindow();
	ENGINE_API CVARManager* GetCvars();
	ENGINE_API IRender* GetRender();
	ENGINE_API IAudio* GetAudio();
	ENGINE_API PhysSystem* GetPhysics();
	ENGINE_API Cache* GetCache();
	ENGINE_API GUI* GetGUI();
	ENGINE_API Scene* GetScene();
	ENGINE_API IGame* GetGame();
	ENGINE_API IScriptInterp* GetScripting();
}