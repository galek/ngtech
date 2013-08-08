/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//***************************************************************************
#include "ILSystem.h"
#include "Log.h"
//***************************************************************************

namespace VEGA {


/*
*/
	ILSystem::ILSystem() {
		Log::writeHeader("-- ILSystem --");

		ilInit();
		iluInit();
	}


/*
*/
	ILSystem::~ILSystem() {
	}

}