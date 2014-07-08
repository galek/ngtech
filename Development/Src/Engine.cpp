/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    Engine.cpp
* Desc:    Engine manager impl.
* Version: 1.01
* Author:  Nick Galko <galek@vegaengine.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/
#include "EnginePrivate.h"
//***************************************************
#include "Engine.h"
#include "CVARManager.h"
#include "WindowSystem.h"
#include "GLSystem.h"
#include "ILSystem.h"
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



namespace VEGA {
	using namespace Common;
	Engine engine;

	Engine* GetEngine() {
		return &engine;
	}

#define ENGINE_VERSION_NUMBER 0.3.3
#define ENGINE_VERSION_STRING "0.3.3"
	/*
	*/
	Engine::Engine()
	{
		log = new Log();
		std::string logstring = "Engine Version:"ENGINE_VERSION_STRING;
		logstring += " Build Date : "__DATE__;
		logstring += " : "__TIME__;
		LogPrintf(logstring);
		_preInit();
	}

	/*
	*/
	void Engine::_preInit()
	{
		vfs = new FileSystem();
		Debug("[Init] FileSystem Finished");
		config = new Config("..\\user.ltx");
		cvars = new CVARManager(config);

		iWindow = new WindowSystem(cvars);
		Debug("[Init] Window Finished");
		iRender = new GLSystem();
		Debug("[Init] Render Finished");
		alSystem = new ALSystem();
		Debug("[Init] Audio Finished");
		ilSystem = new ILSystem();
		Debug("[Init] ImageCodec Finished");
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
	void Engine::initialise()
	{
		if (iWindow){
			iWindow->initialise();
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
		//ilSystem->initialise();
		//Debug("[Init]ImageCodec Finished");
		if (physSystem){
			physSystem->initialise();
			Debug("[Init] Physics Finished");
		}
		//cache->initialise();
		//Debug("[Init]FS Finished");
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

		if(scripting){
			scripting->initialise();
			Debug("[Init] Scripting Finished");
		}
		//initialize Game
		if (game){
			game->initialise();
			Debug("[Init] Game Finished");
		}
		running = true;
		Debug("[Init] All Systems Initialised");
	}

	/*
	*/
	Engine::~Engine()  {
		SAFE_DELETE(scripting);
		SAFE_DELETE(game);
		SAFE_DELETE(gui);
		SAFE_DELETE(scene);
		SAFE_DELETE(physSystem);
		SAFE_DELETE(ilSystem);
		SAFE_DELETE(alSystem);
		SAFE_DELETE(iRender);
		SAFE_DELETE(iWindow);
	}

	/*
	*/
	void Engine::mainLoop() {
		if (this->iWindow)	this->iWindow->update();
		while (this->running)
		{
			if (iWindow)
				this->iWindow->update();

			if ((this->physSystem) && (this->iWindow))
				this->physSystem->update(this->iWindow->getDTime());

			if (this->game->ec)
				this->game->runEventsCallback();
						/*
			if (this->gui)
				this->gui->update();*/

			if (this->iRender)
				this->iRender->clear(GLSystem::COLOR_BUFFER | GLSystem::DEPTH_BUFFER | GLSystem::STENCIL_BUFFER);
			
			if (this->gui)
				this->gui->update();

			if (this->scene)
				this->scene->Update();

			if (this->game->rc)
				this->game->runRenderCallback();

			if (this->iRender)
				this->iRender->flush();

			if (this->iWindow)
				this->iWindow->swapBuffers();

			if (this->game)
				this->game->update();
		}
	}


	/*
	*/	
	void Engine::quit() {
		running = false;
	}
	/*
	*/
	void Engine::_setResources() {
		vfs->addResourceLocation("../data", true);
	}

}