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
#include <TCHAR.h>

// определяем типы
typedef std::map<std::string, std::string> MapString;
typedef std::vector<std::string> VectorString;
typedef std::pair<std::string, std::string> PairString;
typedef std::vector<PairString> VectorStringPairs;
typedef std::basic_string<TCHAR> TString;

typedef signed char	int8;
typedef unsigned char	uint8;
typedef short int	int16;
typedef unsigned short int	uint16;
typedef int	int32;
typedef unsigned int	uint32;
typedef long long	int64;
typedef unsigned long long	uint64;


#define EFORCEINLINE __forceinline


#endif // __MYGUI_TYPES_H__
