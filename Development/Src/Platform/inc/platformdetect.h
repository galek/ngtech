#ifndef PLATFORM_DETECT_H
#define PLATFORM_DETECT_H

///////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////////////////////////
// OS definition

#define PLATFORM_OS_WINDOWS    1
#define PLATFORM_OS_LINUX      2
#define PLATFORM_OS_MACOSX     3

#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( __WIN64__ ) || defined( _WIN64 ) || defined( WIN32 )
#   define IS_OS_WINDOWS    1
#   define IS_OS_LINUX      0
#   define IS_OS_MACOSX     0
#   define PLATFORM_OS      PLATFORM_OS_WINDOWS
#   pragma message("Platform OS is Windows.")
#elif defined(__linux__) || defined( LINUX )
#   define IS_OS_WINDOWS    0
#   define IS_OS_LINUX      1
#   define IS_OS_MACOSX     0
#   define PLATFORM_OS      PLATFORM_OS_LINUX
#   pragma message("Platform OS is Linux.")
#elif ( defined(__APPLE__) && defined(__MACH__) )  || defined( MACOSX )
#   define IS_OS_WINDOWS    0
#   define IS_OS_LINUX      0
#   define IS_OS_MACOSX     1
#   define PLATFORM_OS      PLATFORM_OS_MACOSX
#   pragma message("Platform OS is MacOSX.")
#else
#   error "This platform is not supported."
#endif


#define PLATFORM_COMPILER_MSVC  1
#define PLATFORM_COMPILER_GCC   2

#if defined( _MSC_VER )
#   define PLATFORM_COMPILER            PLATFORM_COMPILER_MSVC
#   define PLATFORM_COMPILER_VERSION    _MSC_VER
#   define IS_COMPILER_MSVC     1
#   define IS_COMPILER_GCC      0
#   pragma message("Platform Compiler is Microsoft Visual C++.")
#	pragma comment(lib,"winmm.lib")
#elif defined( __GNUC__ )
#   define PLATFORM_COMPILER            PLATFORM_COMPILER_GCC
#   define PLATFORM_COMPILER_VERSION    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
#   define IS_COMPILER_MSVC     0
#   define IS_COMPILER_GCC      1
#   pragma message("Platform Compiler is GCC.")
#else
#   error "This compiler is not supported."
#endif


#define PLATFORM_MEMORY_ADDRESS_SPACE_32BIT  1
#define PLATFORM_MEMORY_ADDRESS_SPACE_64BIT  2

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__)
#   define IS_PLATFORM_64BIT                1
#   define IS_PLATFORM_32BIT                0
#   define PLATFORM_MEMORY_ADDRESS_SPACE    PLATFORM_MEMORY_ADDRESS_SPACE_64BIT
#   pragma message("Using 64bit memory address space.")
#else
#   define IS_PLATFORM_64BIT                0
#   define IS_PLATFORM_32BIT                1
#   define PLATFORM_MEMORY_ADDRESS_SPACE    PLATFORM_MEMORY_ADDRESS_SPACE_32BIT
#   pragma message("Using 32bit memory address space.")
#endif

#if IS_OS_WINDOWS
#if !defined(snprintf)
#define snprintf sprintf_s
#endif
#endif

#if IS_OS_WINDOWS
#if !defined(vnsprintf)
#define vnsprintf vsprintf_s
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

#define	_NAME_SEPARATOR_			"|"
#define	_THREADID_NAME_SEPARATOR_	"@"

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)

#endif