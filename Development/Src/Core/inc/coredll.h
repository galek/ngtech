#pragma once

#if PLATFORM_OS != PLATFORM_OS_WINDOWS
#define CORE_API
#elif PLATFORM_OS == PLATFORM_OS_WINDOWS
#ifndef CORE_API
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif
#endif