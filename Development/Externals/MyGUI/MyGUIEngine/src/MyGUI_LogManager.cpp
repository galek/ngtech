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
#include "MyGUI_Precompiled.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_FileLogListener.h"
#include "MyGUI_ConsoleLogListener.h"
#include "MyGUI_LevelLogFilter.h"
#include "MyGUI_LogSource.h"
#include <time.h>

namespace MyGUI
{
	void NGTechPushToLog(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line){
		char buf[32];
		sprintf(buf, "%d", _line);
		std::string line = std::string(buf);
		
		if (_level == MyGUI::LogLevel::Info)
			NGTech::DebugF("[GUI] " + _section + " " + _message + " In: ", _file, _line);
		else if (_level == MyGUI::LogLevel::Warning)
			NGTech::LogPrintf("[GUI] ", _section, " ",_message," In: ",_file, " : ", line);
		else if (_level == MyGUI::LogLevel::Error)
			NGTech::Warning("[GUI] %s %s In:%s : %i",_section.c_str(),_message,_file,line);
		else if (_level == MyGUI::LogLevel::Critical)
			NGTech::Error("[GUI] " + _section + " " + _message + " In: " + _file + " : " + line, true);
	}

} // namespace MyGUI
