#pragma once

#if PLATFORM_OS == PLATFORM_OS_WINDOWS
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryA( a )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif PLATFORM_OS == PLATFORM_OS_MACOSX
#    define DYNLIB_HANDLE CFBundleRef
#    define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#endif

namespace NGTech
{
	//-----------------------------------------------------------------------------
	// Cross-platform dynamic library system
	//-----------------------------------------------------------------------------
	class CORE_API DynLib
	{
	public:
		///
		DynLib(const std::string& name);
		///
		~DynLib();

		/// Load the library
		void Load();
		/// Unload the library
		void Unload();
		/// Get the name of the library
		const std::string& GetName(void) const { return mName; }
		/// Returns the address (handle) of the given symbol from the loaded library.
		void* GetSymbol(const std::string& strName) const throw();
	protected:
		std::string mName;
		/// Gets the last loading error
		std::string GetError(void);
	protected:
		/// Handle to the loaded library.
		DYNLIB_HANDLE m_hInst;
	};

}