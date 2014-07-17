/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//***************************************************************************
#include "../Common/EString.h"
#include "GLSystem.h"
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
		I_Texture *glLogo;
		I_Texture *alLogo;
		I_Texture *ilLogo;
		I_Texture *newtonLogo;
		I_Texture *myGUILogo;

		I_Texture *vegaLogo;

		I_Texture *background;
	};
}