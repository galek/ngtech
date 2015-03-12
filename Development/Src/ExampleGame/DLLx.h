#ifndef DLLX_H
#define DLLX_H

#include "../Platform/inc/platformdetect.h"

#if  PLATFORM_OS == PLATFORM_OS_WINDOWS &&(!defined NGTECH_STATIC_LIBS)
#ifndef GAME_API
#ifdef GAME_EXPORTS
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif
#endif

#else
#ifndef GAME_API
#define GAME_API
#endif
#endif


#endif