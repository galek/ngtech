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
	class VEGA_API Engine : public CoreManager {
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
	VEGA_API Engine* GetEngine();
}