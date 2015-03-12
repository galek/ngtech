#ifndef DLL_DEF
#define DLL_DEF

#include "../../Platform/inc/platformdetect.h"

#if  PLATFORM_OS == PLATFORM_OS_WINDOWS &&(!defined NGTECH_STATIC_LIBS)
#ifndef ENGINE_API
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#endif

#else
#ifndef ENGINE_API
#define ENGINE_API
#endif
#endif


#endif