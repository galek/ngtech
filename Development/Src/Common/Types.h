/*!
@file
@authors	Nick Galko
@date		7/2013
*/
#pragma once

#include <vector>
#include <map>
#include <string>

// определяем типы
typedef std::map<std::string, std::string> MapString;
typedef std::vector<std::string> VectorString;
typedef std::pair<std::string, std::string> PairString;
typedef std::vector<PairString> VectorStringPairs;

#ifndef ENGINE_INLINE
#define ENGINE_INLINE __forceinline
#endif