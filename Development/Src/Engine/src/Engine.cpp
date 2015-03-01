/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
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
#include "ALSystem.h"
#include "Cache.h"
#include "Scene.h"
#include "GUI.h"
#include "VFS.h"
#include "IGame.h"
//***************************************************
#include "EngineScriptInterp.h"
//***************************************************
#include "../../OGLDRV/inc/GLSystem.h"
#include "EngineThreads.h"
//***************************************************
#include "WindowSystem.h"
#include "WindowSystem_GLFW.h"
//***************************************************
#include "Console.h"


namespace NGTech {
	/**
	*/
#define ENGINE_VERSION_NUMBER 0.5
#define ENGINE_VERSION_STRING "0.5"
	const int TICKS_PER_SECOND = 250;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;
	/**
	*/
	void RenderWatermark(I_Texture* _watermark);

	/**
	*/
	Engine::Engine()
	{
		SetCore(this);
		log = new Log();
		LogPrintf("Engine Version: %s Build Date : %s : %s", ENGINE_VERSION_STRING, __DATE__, __TIME__);
	}

	/**
	*/
	void Engine::_preInit()
	{
		Debug("[Init] Engine::_preInit()");

		info = new SystemInfo();
		if (info)
			Debug("[Init] SystemInfo finished");

		LogPrintf(info->getBinaryInfo());
		LogPrintf(info->getSystemInfo());
		LogPrintf(info->getCPUInfo());
		LogPrintf(info->getGPUInfo());
		LogPrintf("GPU Count: %i", info->getGPUCount());
		LogPrintf("GPU Memory: %i", info->getGPUMemory());

		plugins = new EnginePlugins();
		if (!plugins)
			Warning("[Init] EnginePlugins Failed");

		vfs = new FileSystem();
		if (!vfs)
			Warning("[Init] FileSystem Failed");

		config = new Config("../user.ltx");
		if (!config)
			Warning("[Init] Config Failed");

		cvars = new CVARManager(config);
		if (!cvars)
			Warning("[Init] CVARManager Failed");

		// create threads
		threads = new EngineThreads();
		if (!threads)
		Warning("[Init] EngineThreads Failed");

#ifndef DROP_EDITOR
		if (mIsEditor)
		{
			iWindow = new WindowSystem(cvars);
			if (!iWindow)
				Warning("[Init] Window Failed");
		}
		else
#endif
		{
			iWindow = new WindowSystemGLFW(cvars);
			if (!iWindow)
				Warning("[Init] Window Failed");
		}

		iRender = new GLSystem(this);
		if (!iRender)
			Warning("[Init] Render Failed");

		alSystem = new ALSystem();
		if (!alSystem)
			Warning("[Init] Audio Failed");

		physSystem = new PhysSystem(cvars);
		if (!physSystem)
			Warning("[Init] Physics Failed");

		cache = new Cache(cvars);
		if (!cache)
			Warning("[Init] Cache Failed");

		//initialize GUI
		gui = new GUI(cvars);
		if (!gui)
			Warning("[Init] GUI Failed");

		//initialize SceneManager
		scene = new Scene(cvars);
		if (!scene)
			Warning("[Init] SceneManager Failed");

		//initialize Script
		scripting = new EngineScriptInterp();
		if (!scripting)
			Warning("[Init] ScriptInterp Failed");
	}

	/**
	*/
	void Engine::setGame(IGame*_game) {
		Debug("[Init] Engine::setGame()");
		game = _game;
	}

	/**
	*/
	void Engine::initialise(int _hwnd)
	{
		_preInit();
		Debug("[Init] Engine::initialise()");
		if (iWindow) {
			iWindow->initialise(_hwnd);
			Debug("[Init] Window Finished");
		}

		_setResources();
		Debug("[Init] FileSystem Finished");

		if (iRender) {
			iRender->initialise();
			Debug("[Init] Render Finished");
		}

		if (alSystem) {
			alSystem->initialise();
			Debug("[Init] Audio Finished");
		}

		if (physSystem) {
			physSystem->initialise();
			Debug("[Init] Physics Finished");
		}

		//initialize GUI
		if (gui) {
			gui->initialise();
			Debug("[Init] GUI Finished");
		}

		//initialize SceneManager
		if (scene) {
			scene->initialise();
			Debug("[Init] SceneManager Finished");
		}

		if (scripting) {
			scripting->initialise();
			Debug("[Init] Scripting Finished");
		}

		//initialize Game
		if (game) {
			game->initialise();
			Debug("[Init] Game Finished");
		}

		//initialize Console
		console = new Console();
		if (console)
			Debug("[Init] Console Finished");

		this->running = true;

		Debug("[Init] All Systems Initialised");

		mWatermarkTex = iRender->TextureCreate2D("logos/watermark.png");
		if (!mWatermarkTex)
			Warning("[Init] Watermark failed");
	}

	/**
	*/
	Engine::~Engine()
	{
		SAFE_DELETE(mWatermarkTex);
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

	/**
	*/
	void Engine::editorLoop() {
		if (IsRunning()) {
			do_update();
			do_render();
			do_swap();
		}
	}

	/**
	*/
	void Engine::mainLoop()
	{
		PROFILER_SET_CATEGORY("Main");
		PROFILER_ENABLE();
#if LIMITED_FPS
		auto next_game_tick = this->iWindow->GetTicks();
		int loops;

		while (IsRunning()) {

			loops = 0;
			while (this->iWindow->GetTicks() > next_game_tick && loops < MAX_FRAMESKIP) {

				do_update();
				do_render();
				do_swap();

				next_game_tick += SKIP_TICKS;
				loops++;
}
		}
#else
		while (IsRunning())
		{
			PROFILER_START(ENGINE_mainLoop);
			do_update();
			do_render();
			do_swap();
			PROFILER_END();
		}
#endif
	}

	/**
	*/
	void Engine::do_update()
	{
		PROFILER_START(ENGINE_do_update);
		if (iWindow)
			this->iWindow->update();

		PROFILER_START(Game);
		if (this->game)
			if (this->game->ec)
				this->game->runEventsCallback();
		PROFILER_END();

		PROFILER_START(Render);
		if (this->iRender)
			this->iRender->clear(I_Render::COLOR_BUFFER | I_Render::DEPTH_BUFFER | I_Render::STENCIL_BUFFER);
		PROFILER_END();

		PROFILER_START(Scene);
		if (this->scene)
			this->scene->update(paused);
		PROFILER_END();

		PROFILER_START(Audio);
		// run multi-threaded sound
		if (!paused)
			if (this->scene)
				this->scene->runUpdate();//Сейчас обновляем только звук
		PROFILER_END();

		PROFILER_START(Game);
		if (this->game)
			this->game->update();
		PROFILER_END();


		PROFILER_START(Physics);
		// run multi-threaded physics
		if (!paused)
			this->physSystem->runUpdate();
		PROFILER_END();
		PROFILER_END();
	}

	/**
	*/
	void Engine::do_render()
	{
		PROFILER_START(ENGINE_do_render);

		PROFILER_START(Scene_Render);
		if (this->scene)
			this->scene->render();
		PROFILER_END();

		PROFILER_START(GUI);
		if (this->gui)
			this->gui->render();
		PROFILER_END();

		PROFILER_START(Game_Callbacks);
		if (this->game)
			if (this->game->rc)
				this->game->runRenderCallback();
		PROFILER_END();

		if (mWatermarkTex)
			RenderWatermark(mWatermarkTex);

		PROFILER_START(Game_Render);
		if (this->game)
			this->game->render();
		PROFILER_END();

		if (this->iRender)
			this->iRender->flush();

		PROFILER_END();
	}

	/**
	*/
	void Engine::do_swap()
	{
		PROFILER_START(ENGINE_do_swap);

		PROFILER_START(physSystem_waitUpdate);
		// wait multi-threaded physics
		if (this->physSystem)
			this->physSystem->waitUpdate();
		PROFILER_END();

		// wait multi-threaded sound
		PROFILER_START(scene_waitUpdate);
		if (this->scene)
			this->scene->waitUpdate();
		PROFILER_END();

		PROFILER_START(render_endFrame);
		if (this->iRender)
			this->iRender->endFrame();
		PROFILER_END();

		PROFILER_END();
	}

	/**
	*/
	void Engine::quit() {
		PROFILER_DISABLE();
		running = false;
	}

	/**
	*/
	void Engine::_setResources() {
		vfs->addResourceLocation("../data/", true);
	}

	/**
	*/
	float Engine::GetLastFPS() {
		return iWindow->getLastFPS();
	}

	/**
	*/
	float Engine::GetTimePerFrame()
	{
		float mTime = iWindow->getDTime();
		if (mTime > 0)
			return 1000 / iWindow->getDTime();
		return 1.0f;
	}

	/**
	*/
	void Engine::LoadEngineModule(const char*_name) {
		plugins->LoadEngineModule(_name);
	}

	/**
	*/
	void Engine::ConsoleShow(bool _value) {
		console->setVisible(_value);
	}

	/**
	*/
	bool Engine::ConsoleIsActive() {
		return console->getVisible();
	}

	/**
	*/
	void RenderWatermark(I_Texture* _watermark)
	{
		GetRender()->disableCulling();
		GetRender()->enableBlending(I_Render::SRC_ALPHA, I_Render::ONE_MINUS_SRC_ALPHA);
		_watermark->set(0);
		GetRender()->setColor(Vec4(1, 1, 1, 1) * 1);

		GetRender()->enable2d(false);
		GetRender()->drawRect(GetCvars()->r_width - (GetCvars()->r_width - 1)/*x*/, GetCvars()->r_height - (GetCvars()->r_height / 6)/*y*/, GetCvars()->r_width - (GetCvars()->r_width - 454)/*x*/, GetCvars()->r_height - (GetCvars()->r_height / 6) + 46/*y*/, 0, 0, 1, 1);
		GetRender()->enable3d();

		_watermark->unset(0);
		GetRender()->disableBlending();
		GetRender()->enableCulling();
	}
}