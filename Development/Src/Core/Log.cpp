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
#include "Error.h"
#include <stdarg.h>
//***************************************************************************

namespace VEGA {

#define LOG_FILE "EngineLog.html"
	void DebugF(String text, const char* _file, int _line){
		char buf[32];
		sprintf(buf, "%d", _line);
		std::string line = std::string(buf);
#ifdef _ENGINE_DEBUG_
		Log::write(text + " In: " + _file + " : " + line);
#endif	
	}

	void Warning(const TCHAR *fmt, ...){

		TCHAR           msg[8000];

        va_list         argptr;
        va_start (argptr,fmt);
        _vstprintf (msg,fmt,argptr);
        va_end (argptr);
        
        if(TString(msg).empty())
                return;

        TString sMsg ="[WARNING]: ";
        sMsg += msg;

		Log::warning(sMsg.c_str());
	}

	void LogPrintf(String text){
		  Log::write(text);	}

	void Error(String text, bool _fatal){
		if (_fatal)	Error::showAndExit(text);
		else Log::error(text);	}
	//---------------------------------------------------------------------------
	//Desc:    creates the log file
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Log::Log() {
		FILE *fLog;
		fLog = fopen(LOG_FILE, "wt");

		// print log header
		fputs("<html><head>\n", fLog);
		fputs("<meta http-equiv=\"Content - Type\" content=\"text / html; charset = windows - 1251\" />\n", fLog);
		fputs("<title>VEGA Engine log</title>\n", fLog);
		fputs("<style type=\"text/css\">\n", fLog);
		fputs("body { background: #061920; padding: 0px; }\n", fLog);
		fputs("h1{font-size: 18pt; font-family: Arial; color: #C9D6D6;margin: 20px;}h2{font-size: 10pt; font-family: Arial; color: #C9D6D6;margin: 0px;padding-top: 10px;}#msg{background-color: #39464C;font-size: 10pt; font-family: Arial; color: white;padding-left: 5px;margin-bottom: 1px;}#warn{background-color: #A68600;font-size: 11pt;font-weight: bold;font-family: Arial; color: white;padding-left: 15px;margin-bottom: 1px;}#err{background-color: maroon;font-size: 11pt;font-weight: bold;font-family: Arial; color: white;padding-left: 15px;margin-bottom: 1px;}</style></head>", fLog);
		fputs("<body><h1>VEGA Engine log</h1>", fLog);
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
		String HTMLAdd = "<div id=\"msg\">";
		HTMLAdd += text + "</div>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		printf("%s", HTMLAdd.c_str());
#endif
		fclose(fLog);
	}
	//---------------------------------------------------------------------------
	//Desc:    writes Header text String to the log file
	//Params:  text - String to write
	//Returns: -
	//---------------------------------------------------------------------------
	void Log::writeHeader(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<h2>";
		HTMLAdd += text + "</h2>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		printf("%s", HTMLAdd.c_str());
#endif
		fclose(fLog);
	}
	//---------------------------------------------------------------------------
	//Desc:    writes warning to the log file
	//Params:  text - String to write
	//Returns: -
	//---------------------------------------------------------------------------
	void Log::warning(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<div id=\"warn\">";
		HTMLAdd += text + "</div>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		printf("%s", HTMLAdd.c_str());
#endif
		fclose(fLog);
	}
	//---------------------------------------------------------------------------
	//Desc:    writes error text to the log file
	//Params:  text - String to write
	//Returns: -
	//---------------------------------------------------------------------------
	void Log::error(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<div id=\"err\">";
		HTMLAdd += text + "</div>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		printf("%s", HTMLAdd.c_str());
#endif
		fclose(fLog);
	}

}