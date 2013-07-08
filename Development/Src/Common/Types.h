/*!
@file
@author		Albert Semenov
@date		11/2007
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
#ifndef __I_TYPES_H__
#define __I_TYPES_H__


#include <vector>
#include <map>
#include <string>

// определяем типы
typedef std::map<std::string, std::string> MapString;
typedef std::vector<std::string> VectorString;
typedef std::pair<std::string, std::string> PairString;
typedef std::vector<PairString> VectorStringPairs;

typedef char int8;
typedef short int16;
typedef int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;

typedef unsigned int Char;


#endif // __MYGUI_TYPES_H__
