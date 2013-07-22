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
#include "..\Core\CoreManager.h"
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


		void mainLoop();

		void quit();
		void initialise();
		void setGame(IGame*_game);
	private:
		void _preInit();
		void _setResources();	
	public:
		struct CVARManager*cvars;
		struct Log* log;
		class Config *config;
		class FileSystem*vfs;
		class ALSystem *alSystem;
		class ILSystem *ilSystem;
		class PhysSystem *physSystem;

		class Cash *cash;
		class GUI*gui;
		class Scene*scene;
		struct IGame*game;
	};
	VEGA_API extern Engine engine;
};