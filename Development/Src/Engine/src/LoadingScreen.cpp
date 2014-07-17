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

namespace NGTech {

	/*
	*/
	LoadingScreen::LoadingScreen(const String &path) {
		
	glLogo = GetRender()->TextureCreate2D("../data/textures/logos/glLogo.png");
	glLogo->setFilter(I_Texture::LINEAR);

	alLogo = GetRender()->TextureCreate2D("../data/textures/logos/alLogo.png");
	alLogo->setFilter(I_Texture::LINEAR);

	ilLogo = GetRender()->TextureCreate2D("../data/textures/logos/ilLogo.png");
	ilLogo->setFilter(I_Texture::LINEAR);

	newtonLogo = GetRender()->TextureCreate2D("../data/textures/logos/newtonLogo.png");
	newtonLogo->setFilter(I_Texture::LINEAR);

	myGUILogo = GetRender()->TextureCreate2D("../data/textures/logos/myguiLogo.png");
	myGUILogo->setFilter(I_Texture::LINEAR);

	background = GetRender()->TextureCreate2D(path);
	}

	LoadingScreen::~LoadingScreen() {}

	void LoadingScreen::show() {
		GetRender()->clear(I_Render::COLOR_BUFFER | I_Render::DEPTH_BUFFER);
		GetRender()->enable2d(true);

		background->set(0);
		GetRender()->drawRect(0, 0, 1, 1, 0, 0, 1, 1);
		background->unset(0);
	
		glLogo->set(0);
		GetRender()->drawRect(0.15, 0.9, 0.3,   1.0, 0, 0, 1, 1);
		glLogo->unset(0);

		alLogo->set(0);
		GetRender()->drawRect(0.3, 0.9, 0.45,   1.0, 0, 0, 1, 1);
		alLogo->unset(0);

		ilLogo->set(0);
		GetRender()->drawRect(0.45, 0.9, 0.6,   1.0, 0, 0, 1, 1);
		ilLogo->unset(0);

		newtonLogo->set(0);
		GetRender()->drawRect(0.6, 0.9, 0.8, 1.0, 0, 0, 1, 1);
		newtonLogo->set(0);

		myGUILogo->set(0);
		GetRender()->drawRect(0.8, 0.9, 1.0, 1.0, 0, 0, 1, 1);
		myGUILogo->set(0);

		GetRender()->enable3d();
		GetRender()->flush();
		GetWindow()->swapBuffers();
	}

}
