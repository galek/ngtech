#pragma once

//***************************************************************************
#include "../Common/StringHelper.h"
#include "IRender.h"
//***************************************************************************

namespace NGTech {

	/**
	Loading screen class
	*/
	class ENGINE_API LoadingScreen {
	public:
		LoadingScreen(const String &path);
		~LoadingScreen();

		/**
		Shows the loading screen
		*/
		void show();

	private:
		I_Texture *background;
	};
}