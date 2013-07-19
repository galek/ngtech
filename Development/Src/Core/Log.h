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
	void Debug(String text);
	void Warning(String text);
	void LogPrintf(String text);
	//---------------------------------------------------------------------------
	//Desc: Log file struct
	//---------------------------------------------------------------------------
	struct Log {
		Log();
		static void write(String text);
		static void printf(const char *format, ...);
	};

};