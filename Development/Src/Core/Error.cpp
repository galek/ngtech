/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "CorePrivate.h"
//**************************************
#include "Error.h"
#include "Log.h"
//**************************************
#ifdef WIN32
#include <windows.h>
#endif
//**************************************

namespace NGTech {

/*
*/
	void Error::showAndExit(const String &text) {
#ifdef WIN32
		Log::error(text);
		MessageBox(NULL, text.c_str(), "NGTech error", MB_OK | MB_ICONERROR);
		exit(0);
#else
		Log::error(text);
		exit(0);
#endif
	}

}