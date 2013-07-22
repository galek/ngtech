/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

//***************************************************
#include "ASTEngine.h"
//***************************************************

#define AST_ENGINE_VERSION 0.2.6

ASTEngine *astEngine;

//---------------------------------------------------------------------------
//Desc:    creates new ASTEngine
//Params:  -
//Returns: pointer to new ASTEngine
//---------------------------------------------------------------------------
ASTEngine *ASTEngine::create() {
	if(astEngine) { return astEngine; };
	
	astEngine = new ASTEngine();

	Log::create();
	Log::write("AST engine 0.2.6\n");

	astEngine->rc = false;
	astEngine->ec = false;

	astEngine->config = Config::create("ast_conf.txt");
		
	int w = astEngine->config->getInt("window_width");
	int h = astEngine->config->getInt("window_height");
	int b = astEngine->config->getInt("window_bpp");
	int z = astEngine->config->getInt("window_zdepth");
	bool f = astEngine->config->getBool("window_fullscreen");

	astEngine->windowSystem  = WindowSystem::create(w, h, b, z, f);
	astEngine->glSystem   = GLSystem::create();
	astEngine->alSystem   = ALSystem::create();
	astEngine->ilSystem   = ILSystem::create();
	astEngine->physSystem = PhysSystem::create();
	

	astEngine->glSystem->requireExtension("GL_ARB_vertex_shader");
	astEngine->glSystem->requireExtension("GL_ARB_fragment_shader");
	astEngine->glSystem->requireExtension("GL_ARB_shader_objects");
	astEngine->glSystem->requireExtension("GL_ARB_shading_language_100");

	astEngine->glSystem->requireExtension("GL_ARB_vertex_buffer_object");
	astEngine->glSystem->requireExtension("GL_EXT_framebuffer_object");

	astEngine->glSystem->requireExtension("GL_ARB_occlusion_query");

	astEngine->glSystem->requireExtension("GL_EXT_texture_filter_anisotropic");
	
	astEngine->running = true;

	return astEngine;
};

//---------------------------------------------------------------------------
//Desc:    returns existing ASTEngine
//Params:  -
//Returns: pointer to existing ASTEngine
//---------------------------------------------------------------------------
ASTEngine* ASTEngine::get() {
	return astEngine;
};

//---------------------------------------------------------------------------
//Desc:    ASTEngine destructor
//Params:  -
//Returns: -
//---------------------------------------------------------------------------
ASTEngine::~ASTEngine()  {
	if(GUI::get()) delete GUI::get();
	if(Scene::get()) delete Scene::get();
	
	delete physSystem;
	delete ilSystem;
	delete alSystem;
	delete glSystem;

	delete windowSystem;
};

//---------------------------------------------------------------------------
//Desc:    engines main loop
//Params:  -
//Returns: -
//---------------------------------------------------------------------------
void ASTEngine::mainLoop() {
	windowSystem->update();

	while(running) {
		windowSystem->update();
		physSystem->update(windowSystem->getDTime());

		if(ec) events_callback();

		glSystem->clear(GLSystem::COLOR_BUFFER | GLSystem::DEPTH_BUFFER | GLSystem::STENCIL_BUFFER);
		
		if(Scene::get()) Scene::get()->draw();
		if(rc) render_callback();
		if(GUI::get()) GUI::get()->draw();

		glSystem->flush();

		windowSystem->swapBuffers();
	};
};

//---------------------------------------------------------------------------
//Desc:    sets render callback
//Params:  callback - pointer to render function
//Returns: -
//---------------------------------------------------------------------------
void ASTEngine::renderCallback(EngineCallback callback) {
	render_callback = callback;
	rc = true;
};

//---------------------------------------------------------------------------
//Desc:    sets events callback
//Params:  callback - pointer to events function
//Returns: -
//---------------------------------------------------------------------------
void ASTEngine::eventsCallback(EngineCallback callback) {
	events_callback = callback;
	ec = true;
};

//---------------------------------------------------------------------------
//Desc:    exits the main loop
//Params:  -
//Returns: -
//---------------------------------------------------------------------------
void ASTEngine::quit() {
	running = false;
};
