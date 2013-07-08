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

namespace NGEngine {


	//---------------------------------------------------------------------------
	//Desc:    creates new ILSystem
	//Params:  -
	//Returns: pointer to new ILSystem
	//---------------------------------------------------------------------------
	ILSystem::ILSystem() {

		Log::write("ILSystem::create()");
		Log::write("{");
		Log::write("}\n");

		ilInit();
		iluInit();
	}


	//---------------------------------------------------------------------------
	//Desc:    ILSystem destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ILSystem::~ILSystem() {
	}

}