/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    Engine.cpp
* Desc:    Engine manager impl.
* Version: 1.01
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#include "EnginePrivate.h"
//***************************************************
#include "Engine.h"
#include "CVARManager.h"
#include "WindowSystem.h"
#include "IRender.h"
#include "ALSystem.h"
#include "PhysSystem.h"
#include "Cache.h"
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
#include "IGame.h"
#include "EngineScriptInterp.h"
//***************************************************
#include "../../OGLDRV/inc/GLSystem.h"
#include "EngineThreads.h"
//***************************************************



namespace NGTech {
#define ENGINE_VERSION_NUMBER 0.3.4
#define ENGINE_VERSION_STRING "0.3.4"
	/*
	*/
	Engine::Engine()
	{
		SetCore(this);
		log = new Log();
		LogPrintf("Engine Version:"ENGINE_VERSION_STRING" Build Date : "__DATE__" : "__TIME__);
		_preInit();
	}

	/*
	*/
	void Engine::_preInit()
	{
		info = new SystemInfo();
		LogPrintf(info->getBinaryInfo());
		LogPrintf(info->getSystemInfo());
		LogPrintf(info->getCPUInfo());
		LogPrintf(info->getGPUInfo());
		plugins = new EnginePlugins();
		vfs = new FileSystem();
		Debug("[Init] FileSystem Finished");
		config = new Config("../user.ltx");
		cvars = new CVARManager(config);
		// create threads
		threads = new EngineThreads();

		iWindow = new WindowSystem(cvars);
		Debug("[Init] Window Finished");
		iRender = new GLSystem();
		Debug("[Init] Render Finished");
		alSystem = new ALSystem();
		Debug("[Init] Audio Finished");
		physSystem = new PhysSystem();
		Debug("[Init] Physics Finished");
		cache = new Cache(cvars);
		Debug("[Init] Cache Finished");
		//initialize GUI
		gui = new GUI(cvars);
		Debug("[Init] GUI Finished");
		//initialize SceneManager
		scene = new Scene(cvars);
		Debug("[Init] SceneManager Finished");
		//initialize Script
		scripting = new EngineScriptInterp();
		Debug("[Init] ScriptInterp Finished");
	}
	/*
	*/
	void Engine::setGame(IGame*_game){
		game = _game;
	}

	/*
	*/
	void Engine::initialise(int _hwnd)
	{
		if (iWindow){
			iWindow->initialise(_hwnd);
			Debug("[Init] Window Finished");
		}

		_setResources();
		Debug("[Init] FileSystem Finished");

		if (iRender){
			iRender->initialise();
			Debug("[Init] Render Finished");
		}

		if (alSystem){
			alSystem->initialise();
			Debug("[Init] Audio Finished");
		}

		if (physSystem){
			physSystem->initialise();
			Debug("[Init] Physics Finished");
		}

		//initialize GUI
		if (gui){
			gui->initialise();
			Debug("[Init] GUI Finished");
		}

		//initialize SceneManager
		if (scene){
			scene->initialise();
			Debug("[Init] SceneManager Finished");
		}

		if (scripting){
			scripting->initialise();
			Debug("[Init] Scripting Finished");
		}

		//initialize Game
		if (game){
			game->initialise();
			Debug("[Init] Game Finished");
		}

		// run threads
		threads->runSound();
		threads->runFileSystem();
		Debug("[Init] Threads Finished");
		this->running = true;
		Debug("[Init] All Systems Initialised");
	}

	/*
	*/
	Engine::~Engine()  {
		if (threads) {
			threads->stopSound();
			threads->stopJobs();
		}
		SAFE_DELETE(scripting);
		SAFE_DELETE(game);
		SAFE_DELETE(gui);
		SAFE_DELETE(scene);
		SAFE_DELETE(physSystem);
		SAFE_DELETE(alSystem);
		SAFE_DELETE(iRender);
		SAFE_DELETE(iWindow);
		SAFE_DELETE(threads);
	}

	/*
	*/
	void Engine::mainLoop() {
		if (this->iWindow)	this->iWindow->update();
		while (this->running)
		{
			updateFrame();
		}
	}

	void Engine::updateFrame() {
			if (iWindow)
				this->iWindow->update();

			if (this->physSystem)
				this->physSystem->update();

			if (this->game->ec)
				this->game->runEventsCallback();

			if (this->iRender)
				this->iRender->clear(I_Render::COLOR_BUFFER | I_Render::DEPTH_BUFFER | I_Render::STENCIL_BUFFER);

			if (this->scene)
				this->scene->Update();

			if (this->gui)
				this->gui->update();

			if (this->game->rc)
				this->game->runRenderCallback();

			if (this->iRender)
				this->iRender->flush();

			if (this->iWindow)
				this->iWindow->swapBuffers();

			if (this->game)
				this->game->update();
	}


	/*
	*/
	void Engine::quit() {
		running = false;
	}

	/*
	*/
	void Engine::_setResources() {
		vfs->addResourceLocation("../data/", true);
	}

	/*
	*/
	float Engine::GetLastFPS() {
		return 1000 / iWindow->getDTime();
	}
	/*
	*/
	void Engine::LoadEngineModule(const char*_name){
		plugins->LoadEngineModule(_name);
	}
}