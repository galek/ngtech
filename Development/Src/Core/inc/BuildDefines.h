#ifndef _BUILD_DEFINES_H
#define _BUILD_DEFINES_H

#if defined (_DEBUG) || defined (_MIXED)
#ifndef _ENGINE_DEBUG_
#define _ENGINE_DEBUG_ 1
#endif
#endif

#ifndef LOG_FILE
#define LOG_FILE "../Logs/EngineLog.html"
#endif

#ifndef HAVE_NVAPI
#define HAVE_NVAPI 1
#endif

#ifndef HAVE_AMDADL
#define HAVE_AMDADL 1
#endif

#ifndef USE_OWN_MINIDUMP
#define USE_OWN_MINIDUMP 1
#endif

//Is depreated?
#ifndef EYEFINITY_SUPPORT
#define EYEFINITY_SUPPORT 1
#endif

#ifndef DROP_EDITOR
//#define DROP_EDITOR 1
#endif

#ifndef USE_STEAMWORKS
//#define USE_STEAMWORKS 1
#endif

#ifndef ENGINE_RELEASE
//#define USE_PROFILER 1
#endif

#ifndef LIMITED_FPS
//#define LIMITED_FPS 1
#endif

#ifndef _DEMO_BUILD
//#define _DEMO_BUILD 1
#endif

#endif