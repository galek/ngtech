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
#include "CoreManager.h"
//***************************************************

namespace NGEngine {
	class ThreadPool;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main class. Created one time
	//---------------------------------------------------------------------------
	class Engine:public CoreManager {
	public:
		Engine();
		static Engine *get();

		~Engine();
/*
		typedef void (*EngineCallback)();*/

		void renderCallback(EngineCallback callback);
		void eventsCallback(EngineCallback callback);

		void mainLoop();

		void quit();
		void initialise();
	private:
		void PreInit();
	private:
		/*EngineCallback render_callback;
		EngineCallback events_callback;
		bool rc, ec;*/

	public:
		CVARManager*cvars;
//Nick:Port		ThreadPool* tp;
		Log* log;
		Config *config;
		ALSystem *alSystem;
		ILSystem *ilSystem;
		PhysSystem *physSystem;

		Cash *cash;
		GUI*gui;
		Scene*scene;

/*
		bool running;*/
	};
	extern Engine engine;
};