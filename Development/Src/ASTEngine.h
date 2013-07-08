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
#include "Log.h"
#include "Config.h"

#include "LoadingScreen.h"

#include "Scene.h"
#include "Object.h"
#include "Terrain.h"
#include "Light.h"
#include "Effect.h"
#include "Camera.h"

#include "Font.h"
#include "GUI.h"
//***************************************************

//---------------------------------------------------------------------------
//Desc: Engine`s main class. Created one time
//---------------------------------------------------------------------------
class ASTEngine {
public:
	static ASTEngine *create();
	static ASTEngine *get();

	~ASTEngine();

	typedef void (*EngineCallback)();

	void renderCallback(EngineCallback callback);
	void eventsCallback(EngineCallback callback);

	void mainLoop();

	void quit();

private:
	EngineCallback render_callback;
	EngineCallback events_callback;
	bool rc, ec;

	WindowSystem *windowSystem;
	GLSystem *glSystem;
	ALSystem *alSystem;
	ILSystem *ilSystem;
	PhysSystem *physSystem;

	Config *config;

	bool running;
};