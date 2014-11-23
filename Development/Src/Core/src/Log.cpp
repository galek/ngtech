/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    Log.cpp
* Desc:    Logger manager impl.
* Version: 1.01
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#include "CorePrivate.h"
//***************************************************************************
#if (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
#include <sys/stat.h>
#elif  PLATFORM_OS == PLATFORM_OS_WINDOWS
#include <direct.h> //_mkdir
#include <tchar.h>
#endif
#include <stdarg.h>
//***************************************************************************
#include "Log.h"
#include "FileHelper.h"
#include "Error.h"
//***************************************************************************

namespace NGTech {
	void DebugF(String text, const char* _file, int _line){
		char buf[32];
		sprintf(buf, "%d", _line);
		std::string line = std::string(buf);
#ifdef _ENGINE_DEBUG_
		Log::write(text + " In: " + _file + " : " + line);
#endif	
	}

	void Warning(const char *fmt, ...){

		char           msg[8000];

		va_list         argptr;
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

		if (String(msg).empty())
			return;

		String sMsg = "[WARNING]: ";
		sMsg += msg;

		Log::warning(sMsg.c_str());
	}

	void DebugM(const char *fmt, ...){

		char           msg[8000];

		va_list         argptr;
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

		if (String(msg).empty())
			return;

#ifdef _ENGINE_DEBUG_
		Log::write(msg);
#endif
	}

	void LogPrintf(const char *fmt, ...){

		char           msg[8000];

		va_list         argptr;
		va_start(argptr, fmt);
		vsprintf(msg, fmt, argptr);
		va_end(argptr);

		if (String(msg).empty())
			return;

		Log::write(msg);
	}

	void Error(String text, bool _fatal){
		if (_fatal)	Error::showAndExit(text);
		else Log::error(text);
	}

	Log::Log() {
#if (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
		//with read/write/search permissions for owner and group, and with read/search permissions for others.
		mkdir("../Logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#elif PLATFORM_OS == PLATFORM_OS_WINDOWS
		_mkdir("../Logs");
#endif
		FILE *fLog;
		fLog = fopen(LOG_FILE, "wt");

		if (fLog)
		{
			// print log header
			fputs("<html><head>\n", fLog);
			fputs("<meta http-equiv=\"Content - Type\" content=\"text / html; charset = windows - 1251\" />\n", fLog);
			fputs("<title>NGTech log</title>\n", fLog);
			fputs("<style type=\"text/css\">\n", fLog);
			fputs("body { background: #061920; padding: 0px; }\n", fLog);
			fputs("h1{font-size: 18pt; font-family: Arial; color: #C9D6D6;margin: 20px;}h2{font-size: 10pt; font-family: Arial; color: #C9D6D6;margin: 0px;padding-top: 10px;}#msg{background-color: #39464C;font-size: 10pt; font-family: Arial; color: white;padding-left: 5px;margin-bottom: 1px;}\
				  				  			  			  			  			  			  #warn{background-color: #A68600;font-size: 11pt;font-weight: bold;font-family: Arial; color: white;padding-left: 15px;margin-bottom: 1px;}\
																							  																			  															  												  									  						  			  #prof{background-color: #ADD8E6;font-size: 11pt;font-weight: bold;font-family: Arial; color: black;padding-left: 15px;margin-bottom: 1px;}\
																																																																																							  																																																																  																																													  																														  																		  									  			  #err{background-color: maroon;font-size: 11pt;font-weight: bold;font-family: Arial; color: white;padding-left: 15px;margin-bottom: 1px;}</style></head>", fLog);
			fputs("<body><h1>NGTech log</h1>", fLog);
			fclose(fLog);
		}
	}

	void Log::write(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<div id=\"msg\">";
		HTMLAdd += text + "</div>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		if (fLog)
			fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		if (fLog)
			printf("%s", HTMLAdd.c_str());
#endif
		if (fLog)
			fclose(fLog);
	}

	void Log::writeHeader(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<h2>";
		HTMLAdd += text + "</h2>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		if (fLog)
			fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		if (fLog)
			printf("%s", HTMLAdd.c_str());
#endif
		if (fLog)
			fclose(fLog);
	}

	void Log::profiler(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<div id=\"prof\">";
		HTMLAdd += text + "</div>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		if (fLog)
			fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		if (fLog)
			printf("%s", HTMLAdd.c_str());
#endif
		if (fLog)
			fclose(fLog);
	}

	void Log::warning(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<div id=\"warn\">";
		HTMLAdd += text + "</div>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		if (fLog)
			fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		if (fLog)
			printf("%s", HTMLAdd.c_str());
#endif
		if (fLog)
			fclose(fLog);
	}

	void Log::error(String text) {
		text += '\n';
		FILE *fLog;
		String HTMLAdd = "<div id=\"err\">";
		HTMLAdd += text + "</div>\n";
		fLog = fopen(LOG_FILE, "a+t");
#ifdef WIN32
		if (fLog)
			fprintf(fLog, "%s", HTMLAdd.c_str());
#else
		if (fLog)
			printf("%s", HTMLAdd.c_str());
#endif
		if (fLog)
			fclose(fLog);
	}

}