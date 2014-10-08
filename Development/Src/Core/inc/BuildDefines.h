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

#ifndef HAVE_NVCPL
#define HAVE_NVCPL 1
#endif

#ifndef HAVE_ATIMGPU
//#define HAVE_ATIMGPU 1
#endif

#ifndef USE_OWN_MINIDUMP
#define USE_OWN_MINIDUMP 1
#endif

#ifndef EYEFINITY_SUPPORT
#define EYEFINITY_SUPPORT 1
#endif

#ifndef DROP_EDITOR
//#define DROP_EDITOR 1
#endif

#ifndef USE_STEAMWORKS
//#define USE_STEAMWORKS 1
#endif

#endif