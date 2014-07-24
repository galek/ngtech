/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

#include "coredll.h"
//***************************************************************************
#include "../../Common/EString.h"
//***************************************************************************

namespace NGTech {
#define Debug(text) DebugF(text, __FILE__, __LINE__)
	void CORE_API DebugF(String text, const char* _file, int _line);
	void CORE_API Warning(const char *fmt, ...);
	void CORE_API DebugM(const char *fmt, ...);
	void CORE_API LogPrintf(const char *fmt, ...);
	void CORE_API Error(String text, bool _fatal);
	//---------------------------------------------------------------------------
	//Desc: Log file struct
	//---------------------------------------------------------------------------
	struct CORE_API Log {
		Log();
		static void write(String text);
		static void warning(String text);
		static void error(String text);
		static void writeHeader(String text);
	};

};