/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once 

//***************************************************
#include "WindowSystem.h"
#include "GLSystem.h"
#include "ILSystem.h"
#include "ALSystem.h"
#include "PhysSystem.h"
#include "Cash.h"
#include "Log.h"
#include "Config.h"
#include "LoadingScreen.h"
#include "Scene.h"
#include "Object.h"
#include "Terrain.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "Font.h"
#include "GUI.h"
#include "VFS.h"
//***************************************************
#include "CoreManager.h"
//***************************************************
#include "tbb/blocked_range.h"
//***************************************************

namespace NGEngine {
	struct IGame;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main class. Created one time
	//---------------------------------------------------------------------------
	class Engine:public CoreManager {
	public:
		Engine();

		~Engine();


		void mainLoop();

		void quit();
		void initialise();
		void SetGame(IGame*_game);
	private:
		void _PreInit();
		void _SetResources();	
	public:
		CVARManager*cvars;
		Log* log;
		Config *config;
		class FileSystem*vfs;
		ALSystem *alSystem;
		ILSystem *ilSystem;
		PhysSystem *physSystem;

		Cash *cash;
		GUI*gui;
		Scene*scene;
		IGame*game;
	};
	extern Engine engine;
};