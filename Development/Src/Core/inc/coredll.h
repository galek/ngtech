#pragma once

#include "../../Platform/inc/platformdetect.h"

#if  PLATFORM_OS == PLATFORM_OS_WINDOWS &&(!defined NGTECH_STATIC_LIBS)
#ifndef CORE_API
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif

#else
#ifndef CORE_API
#define CORE_API
#endif
#endif