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

namespace VEGA {

	/*
	*/
	LoadingScreen::LoadingScreen(const String &path) {
		
	glLogo = GLTexture::create2d("../data/textures/logos/glLogo.png");
	glLogo->setFilter(GLTexture::LINEAR);

	alLogo = GLTexture::create2d("../data/textures/logos/alLogo.png");
	alLogo->setFilter(GLTexture::LINEAR);

	ilLogo = GLTexture::create2d("../data/textures/logos/ilLogo.png");
	ilLogo->setFilter(GLTexture::LINEAR);

	newtonLogo = GLTexture::create2d("../data/textures/logos/newtonLogo.png");
	newtonLogo->setFilter(GLTexture::LINEAR);

	myGUILogo = GLTexture::create2d("../data/textures/logos/myguiLogo.png");
	myGUILogo->setFilter(GLTexture::LINEAR);


	//vegaLogo = GLTexture::create2d("../data/textures/logos/vegaLogo.png");

	background = GLTexture::create2d(path);
	}

	LoadingScreen::~LoadingScreen() {}

	void LoadingScreen::show() {
		GetEngine()->iRender->clear(GLSystem::COLOR_BUFFER | GLSystem::DEPTH_BUFFER);
		GetEngine()->iRender->enable2d(true);

		background->set(0);
		GetEngine()->iRender->drawRect(0, 0, 1, 1, 0, 0, 1, 1);
		background->unset(0);

		/*vegaLogo->set(0);
		GetEngine()->iRender->drawRect(0.0, 0.0, 0.3, 0.1, 0, 0, 1, 1);
		vegaLogo->unset(0);*/

		glLogo->set(0);
		GetEngine()->iRender->drawRect(0.15, 0.9, 0.3,   1.0, 0, 0, 1, 1);
		glLogo->unset(0);

		alLogo->set(0);
		GetEngine()->iRender->drawRect(0.3, 0.9, 0.45,   1.0, 0, 0, 1, 1);
		alLogo->unset(0);

		ilLogo->set(0);
		GetEngine()->iRender->drawRect(0.45, 0.9, 0.6,   1.0, 0, 0, 1, 1);
		ilLogo->unset(0);

		newtonLogo->set(0);
		GetEngine()->iRender->drawRect(0.6, 0.9, 0.8, 1.0, 0, 0, 1, 1);
		newtonLogo->set(0);

		myGUILogo->set(0);
		GetEngine()->iRender->drawRect(0.8, 0.9, 1.0, 1.0, 0, 0, 1, 1);
		myGUILogo->set(0);

		GetEngine()->iRender->enable3d();
		GetEngine()->iRender->flush();
		GetEngine()->iWindow->swapBuffers();
	}

}
