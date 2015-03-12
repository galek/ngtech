#pragma once

#include "../../Platform/inc/platformdetect.h"

#if (PLATFORM_OS == PLATFORM_OS_WINDOWS) &&(!defined NGTECH_STATIC_LIBS)
#ifndef RENDER_API
#ifdef RENDER_EXPORTS
#define RENDER_API __declspec(dllexport)
#else
#define RENDER_API __declspec(dllimport)
#endif
#endif

#else
#ifndef RENDER_API
#define RENDER_API
#endif
#endif