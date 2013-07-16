/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//***************************************************
#include "Engine.h"
#include "CVARManager.h"
//***************************************************
#include "IGame.h"

namespace NGEngine {
	using namespace Common;
	Engine engine;
#define ENGINE_VERSION_NUMBER 0.2.81
#define ENGINE_VERSION_STRING "0.2.81"
	//---------------------------------------------------------------------------
	//Desc:    creates new NGEngine
	//Params:  -
	//Returns: pointer to new NGEngine
	//---------------------------------------------------------------------------
	Engine::Engine():
	cvars(nullptr),
		log(nullptr),
		config(nullptr),
		alSystem(nullptr),
		ilSystem(nullptr),
		physSystem(nullptr),
		cash(nullptr),
		gui(nullptr),
		scene(nullptr)
	{
		log=new Log();
		Log::write("NG Engine "ENGINE_VERSION_STRING"\n");
		_PreInit();
	}

	//---------------------------------------------------------------------------
	//Desc:    Init engine sub-systems
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Engine::_PreInit()
	{
		config = new Config("ast_conf.txt");
		cvars=new CVARManager(config);

		iWindow  = new WindowSystem(cvars);
		Debug("[Init]Window Finished");
		iRender   = new GLSystem();
		Debug("[Init]Render Finished");
		alSystem   = new ALSystem();
		Debug("[Init]Audio Finished");
		ilSystem   = new ILSystem();
		Debug("[Init]ImageCodec Finished");
		physSystem = new PhysSystem();
		Debug("[Init]Physics Finished");
		cash       = new Cash();
		Debug("[Init]FS Finished");
		//initialize GUI
		gui=new GUI();
		Debug("[Init]GUI Finished");
		//initialize SceneManager
		scene=new Scene();
		Debug("[Init]SceneManager Finished");
	}

	void Engine::SetGame(IGame*_game){
	game=_game;
	}

	//---------------------------------------------------------------------------
	//Desc:    Init engine sub-systems
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Engine::initialise()
	{
		if(iWindow)
			iWindow->initialise();
		Debug("[Init]Window Finished");
		if(iRender)
			iRender->initialise();
		Debug("[Init]Render Finished");
		//alSystem->initialise();
		Debug("[Init]Audio Finished");
		//ilSystem->initialise();
		Debug("[Init]ImageCodec Finished");
		//physSystem->initialise();
		Debug("[Init]Physics Finished");
		//cash->initialise();
		Debug("[Init]FS Finished");
		//initialize GUI
		if(gui)
			gui->Initialise();
		Debug("[Init]GUI Finished");
		//initialize SceneManager
		if(scene)
			scene->initialise();
		Debug("[Init]SceneManager Finished");
		//initialize Game
		if(game)
			game->initialise();
		Debug("[Init]Game Finished");

		running = true;
		Debug("[Init]Checking Render Extensions");

		iRender->requireExtension("GL_ARB_vertex_shader");
		iRender->requireExtension("GL_ARB_fragment_shader");
		iRender->requireExtension("GL_ARB_shader_objects");
		iRender->requireExtension("GL_ARB_shading_language_100");
		iRender->requireExtension("GL_ARB_vertex_buffer_object");
		iRender->requireExtension("GL_EXT_framebuffer_object");
		iRender->requireExtension("GL_ARB_occlusion_query");
		iRender->requireExtension("GL_EXT_texture_filter_anisotropic");

		Debug("[Init]All Systems Initialised");
	}

	//---------------------------------------------------------------------------
	//Desc:    NGEngine destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Engine::~Engine()  {
		if(gui) delete gui;
		if(scene) delete scene;

		delete physSystem;
		delete ilSystem;
		delete alSystem;
		delete iRender;

		delete iWindow;
	}
	//---------------------------------------------------------------------------
	//Desc:    engines main loop
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Engine::mainLoop() {
		if(this->iWindow)	this->iWindow->update();
			while(this->running)
		{
			if(iWindow)
				this->iWindow->update();

			if((this->physSystem)&&(this->iWindow)) 
				this->physSystem->update(this->iWindow->getDTime());

			if(game) 
				game->RunEventsCallback();

			if(this->iRender)	
				this->iRender->clear(GLSystem::COLOR_BUFFER | GLSystem::DEPTH_BUFFER | GLSystem::STENCIL_BUFFER);

			if(this->scene)
				this->scene->Update();

			if(game->rc) 
				game->RunRenderCallback();

			if(this->gui) 
				this->gui->Update();

			if(this->iRender)	
				this->iRender->flush();

			if(this->iWindow)	
				this->iWindow->swapBuffers();
			
			if(this->game)
				this->game->Update();
		}
	}
	

	//---------------------------------------------------------------------------
	//Desc:    exits the main loop
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Engine::quit() {
		running = false;
	}

}