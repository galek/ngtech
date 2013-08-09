/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once 

//***************************************************
#include "Engine\DLLDef.h"
//***************************************************
#include "Core\CoreManager.h"
//***************************************************

namespace VEGA {
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
};