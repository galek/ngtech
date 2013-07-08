/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//***************************************************************************
#include "Engine.h"
#include "LoadingScreen.h"
#include "WindowSystem.h"
//***************************************************************************

namespace NGEngine {

	//---------------------------------------------------------------------------
	//Desc:    creates the log file
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	LoadingScreen *LoadingScreen::create(const String &path) {
		LoadingScreen *screen = new LoadingScreen();

		screen->glLogo = GLTexture::create2d("data/textures/logos/glLogo.jpg");
		screen->glLogo->setFilter(GLTexture::LINEAR);

		screen->alLogo = GLTexture::create2d("data/textures/logos/alLogo.jpg");
		screen->alLogo->setFilter(GLTexture::LINEAR);

		screen->ilLogo = GLTexture::create2d("data/textures/logos/ilLogo.jpg");
		screen->ilLogo->setFilter(GLTexture::LINEAR);

		screen->newtonLogo = GLTexture::create2d("data/textures/logos/newtonLogo.jpg");
		screen->newtonLogo->setFilter(GLTexture::LINEAR);


		screen->astLogo = GLTexture::create2d("data/textures/logos/astLogo.jpg");

		screen->background = GLTexture::create2d(path);
		return screen;
	}

	LoadingScreen::~LoadingScreen() {}

	void LoadingScreen::show() {
		engine.iRender->clear(GLSystem::COLOR_BUFFER | GLSystem::DEPTH_BUFFER);
		engine.iRender->enable2d(true);

		background->set(0);
		engine.iRender->drawRect(0, 0, 1, 1, 0, 0, 1, 1);
		background->unset(0);

		astLogo->set(0);
		engine.iRender->drawRect(0.0, 0.0, 0.3, 0.1, 0, 0, 1, 1);
		astLogo->unset(0);

		glLogo->set(0);
		engine.iRender->drawRect(0.15, 0.9, 0.3,   1.0, 0, 0, 1, 1);
		glLogo->unset(0);

		alLogo->set(0);
		engine.iRender->drawRect(0.3, 0.9, 0.45,   1.0, 0, 0, 1, 1);
		alLogo->unset(0);

		ilLogo->set(0);
		engine.iRender->drawRect(0.45, 0.9, 0.6,   1.0, 0, 0, 1, 1);
		ilLogo->unset(0);

		newtonLogo->set(0);
		engine.iRender->drawRect(0.6, 0.9, 0.8,   1.0, 0, 0, 1, 1);
		newtonLogo->set(0);

		engine.iRender->enable3d();
		engine.iRender->flush();
		engine.iWindow->swapBuffers();
	}

}
