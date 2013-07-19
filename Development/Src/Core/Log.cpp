/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "CorePrivate.h"
//***************************************************************************
#include "Log.h"
#include "File.h"
#include <stdarg.h>
//***************************************************************************

namespace VEGA {

#define LOG_FILE "EngineLog.log"
	void Debug(String text){
#ifdef _ENGINE_DEBUG_
		 Log::write(text);
#endif	
	}

	void Warning(String text){
		  Log::write(text);	}

	void LogPrintf(String text){
		  Log::write(text);	}

	//---------------------------------------------------------------------------
	//Desc:    creates the log file
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Log::Log() {
		FILE *fLog;
		fLog = fopen(LOG_FILE, "wt");
		fclose(fLog);
	}

	//---------------------------------------------------------------------------
	//Desc:    writes text String to the log file
	//Params:  text - String to write
	//Returns: -
	//---------------------------------------------------------------------------
	void Log::write(String text) {
		text += '\n';
		FILE *fLog;
		fLog = fopen(LOG_FILE, "a+t");
		fprintf(fLog, "%s", text.c_str());
		printf("%s", text.c_str());
		fclose(fLog);
	}

	void Log::printf(const char *format, ...) {
		/*FILE *fLog;
		fLog = fopen(LOG_FILE, "a+t");

		va_list arg;
		va_start(arg, format);

		vfprintf(fLog, format, arg);

		va_end(arg);

		fclose(fLog);*/
	}

}