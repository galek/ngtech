/*!
@file
@authors	Albert Semenov(MyGUI),Nick Galko
@date		7/2013
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

#define EFORCEINLINE __forceinline


#endif // __MYGUI_TYPES_H__
