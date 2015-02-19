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

#define PLATFORM_OS_WINDOWS     1
#define PLATFORM_OS_LINUX       2
#define PLATFORM_OS_MACOSX      3
#define PLATFORM_OS_ANDROID     4

#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( __WIN64__ ) || defined( _WIN64 ) || defined( WIN32 )
#   define IS_OS_WINDOWS    1
#   define IS_OS_LINUX      0
#   define IS_OS_MACOSX     0
#   define PLATFORM_OS      PLATFORM_OS_WINDOWS
#   pragma message("Platform OS is Windows.")
#elif defined(__linux__) && (!defined(__ANDROID__) )|| defined( LINUX )
#   define IS_OS_WINDOWS    0
#   define IS_OS_LINUX      1
#   define IS_OS_MACOSX     0
#   define DROP_EDITOR      1
#   define PLATFORM_OS      PLATFORM_OS_LINUX
#   pragma message("Platform OS is Linux.")
#elif ( defined(__APPLE__) && defined(__MACH__) )  || defined( MACOSX )
#   define IS_OS_WINDOWS    0
#   define IS_OS_LINUX      0
#   define IS_OS_MACOSX     1
#   define DROP_EDITOR      1
#   define PLATFORM_OS      PLATFORM_OS_MACOSX
#   pragma message("Platform OS is MacOSX.")
#elif defined(__linux__) && (defined(__ANDROID__) )
#   define IS_OS_WINDOWS    0
#   define IS_OS_LINUX      0
#   define IS_OS_MACOSX     0
#   define DROP_EDITOR      1
#   define PLATFORM_OS      PLATFORM_OS_ANDROID
#   pragma message("Platform OS is Android.")
#else
#   error "This platform is not supported."
#endif


#define PLATFORM_COMPILER_MSVC    1
#define PLATFORM_COMPILER_GCC     2
#define PLATFORM_COMPILER_CLANG   3

#if defined( _MSC_VER )
#   define PLATFORM_COMPILER            PLATFORM_COMPILER_MSVC
#   define PLATFORM_COMPILER_VERSION    _MSC_VER
#   define IS_COMPILER_MSVC       1
#   define IS_COMPILER_GCC        0
#   define IS_COMPILER_CLANG      0
#   define IS_COMPILER_INTEL      0
#   define ENGINE_INLINE		  __forceinline
#   pragma message("Platform Compiler is Microsoft Visual C++.")
#	pragma comment(lib,"winmm.lib")
#elif defined( __GNUC__ )
#   define PLATFORM_COMPILER            PLATFORM_COMPILER_GCC
#   define PLATFORM_COMPILER_VERSION    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
#   define IS_COMPILER_MSVC       0
#   define IS_COMPILER_GCC        1
#   define IS_COMPILER_CLANG      0
#   define IS_COMPILER_INTEL      0
#   define ENGINE_INLINE		  inline
#   pragma message("Platform Compiler is GCC.")
#elif defined( __CLANG__ )
#   define PLATFORM_COMPILER            PLATFORM_COMPILER_CLANG
#   define PLATFORM_COMPILER_VERSION    (__CLANG_MAJOR__ + __CLANG_MINOR__ + __CLANG_PATCHLEVEL__)
#   define IS_COMPILER_MSVC       0
#   define IS_COMPILER_GCC        0
#   define IS_COMPILER_CLANG      1
#   define IS_COMPILER_INTEL      0
#   define ENGINE_INLINE		  inline
#   pragma message("Platform Compiler is CLANG.")
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

#if !defined(PTRDIFF_T_DEFINED) && !defined(_PTRDIFF_T)
# ifdef _WIN64
# ifdef GNUC
typedef long long ptrdiff_t;
# else
typedef __int64 ptrdiff_t;
# endif
# else
typedef int ptrdiff_t;
# endif
# endif

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
#define ALIGNED4(VALUE) (((size_t)(VALUE) + 3) & ~3)
#define ALIGNED8(VALUE) (((size_t)(VALUE) + 7) & ~7)
#define ALIGNED16(VALUE) (((size_t)(VALUE) + 15) & ~15)
#define ALIGNED128(VALUE) (((size_t)(VALUE) + 127) & ~127)
#define IS_ALIGNED4(VALUE) (((size_t)(VALUE) & 3) == 0)
#define IS_ALIGNED8(VALUE) (((size_t)(VALUE) & 7) == 0)
#define IS_ALIGNED16(VALUE) (((size_t)(VALUE) & 15) == 0)
#define IS_ALIGNED128(VALUE) (((size_t)(VALUE) & 127) == 0)
#define ASSERT_ALIGNED4(VALUE) assert(IS_ALIGNED4(VALUE))
#define ASSERT_ALIGNED8(VALUE) assert(IS_ALIGNED8(VALUE))
#define ASSERT_ALIGNED16(VALUE) assert(IS_ALIGNED16(VALUE))
#define ASSERT_ALIGNED128(VALUE) assert(IS_ALIGNED128(VALUE))

/*
*/
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
#define ATTRIBUTE_ALIGNED4(NAME) __declspec(align(4)) NAME
#define ATTRIBUTE_ALIGNED8(NAME) __declspec(align(8)) NAME
#define ATTRIBUTE_ALIGNED16(NAME) __declspec(align(16)) NAME
#define ATTRIBUTE_ALIGNED128(NAME) __declspec(align(128)) NAME
#else
#define ATTRIBUTE_ALIGNED4(NAME) NAME __attribute__ ((aligned(4)))
#define ATTRIBUTE_ALIGNED8(NAME) NAME __attribute__ ((aligned(8)))
#define ATTRIBUTE_ALIGNED16(NAME) NAME __attribute__ ((aligned(16)))
#define ATTRIBUTE_ALIGNED128(NAME) NAME __attribute__ ((aligned(128)))
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////

#define	_NAME_SEPARATOR_			"|"
#define	_THREADID_NAME_SEPARATOR_	"@"

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
#define SAFE_DELETE_ARRAY(p) { if (p) { delete [] p; (p) = NULL; } }

#endif