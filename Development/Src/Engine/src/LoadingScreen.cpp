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
		background = GetRender()->TextureCreate2D(path);
		background->setFilter(I_Texture::LINEAR);
	}

	LoadingScreen::~LoadingScreen() {
		SAFE_DELETE(background);
	}

	void LoadingScreen::show() {
		GetRender()->clear(I_Render::COLOR_BUFFER | I_Render::DEPTH_BUFFER);
		GetRender()->enable2d(true);

		background->set(0);
		GetRender()->drawRect(0, 0, 1, 1, 0, 0, 1, 1);
		background->unset(0);

		GetRender()->enable3d();
		GetRender()->flush();
		GetRender()->swapBuffers();
	}

}
