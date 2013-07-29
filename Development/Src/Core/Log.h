/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//***************************************************************************
#include "..\Common\EString.h"
//***************************************************************************

namespace VEGA {
#define Debug(text) DebugF(text, __FILE__, __LINE__)
	void DebugF(String text, const char* _file, int _line);
	void Warning(String text);
	void LogPrintf(String text);
	void Error(String text, bool _fatal);
	//---------------------------------------------------------------------------
	//Desc: Log file struct
	//---------------------------------------------------------------------------
	struct Log {
		Log();
		static void write(String text);
		static void warning(String text);
		static void error(String text);
		static void writeHeader(String text);
	};

};