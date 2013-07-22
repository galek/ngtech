/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once
 
//***************************************************************************
#include "Common/EString.h"
#include "GLSystem.h"
//***************************************************************************

namespace VEGA {

//---------------------------------------------------------------------------
//Desc: Loading screen class
//---------------------------------------------------------------------------
	class VEGA_API LoadingScreen {
public:
	static LoadingScreen *create(const String &path);
	~LoadingScreen();
	
	void show();

private:
	GLTexture *glLogo;
	GLTexture *alLogo;
	GLTexture *ilLogo;
	GLTexture *newtonLogo;

	GLTexture *astLogo;

	GLTexture *background;
};

};