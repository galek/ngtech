/*!
	@file
	@author		Albert Semenov
	@date		01/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_LOG_MANAGER_H__
#define __MYGUI_LOG_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_LogStream.h"
#include "MyGUI_LogSource.h"
#include "MyGUI_Diagnostic.h"
#include <vector>
#include <TCHAR.H>

namespace VEGA {
	void DebugF(std::string text, const char* _file, int _line);
	void Warning(const TCHAR* text, ... );
	void LogPrintf(std::string text);
	void Error(std::string text,bool _fatal);
}

namespace MyGUI
{
	void VEGAPushToLog(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line);

	#define MYGUI_LOGGING(section, level, text) \
	VEGAPushToLog(section, MyGUI::LogLevel::level, MyGUI::LogStream() << text << MyGUI::LogStream::End(), __FILE__, __LINE__)

} // namespace MyGUI

#endif // __MYGUI_LOG_MANAGER_H__
