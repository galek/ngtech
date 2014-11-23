#include "CorePrivate.h"
#if  (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
#include <dlfcn.h>
#endif

namespace NGTech
{
	//-----------------------------------------------------------------------
	DynLib::DynLib(const std::string& name)
	{
		mName = name;
		m_hInst = NULL;
	}

	//-----------------------------------------------------------------------
	DynLib::~DynLib()
	{
	}

	//-----------------------------------------------------------------------
	void DynLib::Load()
	{
		// Log library load
		LogPrintf("Loading library %s", mName.c_str());

		std::string name = mName;
#if (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
		// dlopen() does not add .so to the filename, like windows does for .dll
		if (name.substr(name.length() - 3, 3) != ".so")
			name += ".so";
#endif

		m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD(name.c_str());

		if (!m_hInst)
			Warning("Could not load dynamic library %s.  System Error: %s DynLib::load", mName.c_str(), GetError().c_str());

	}

	//-----------------------------------------------------------------------
	void DynLib::Unload()
	{
		// Log library unload
		LogPrintf("Unloading library %s", mName.c_str());

		if (DYNLIB_UNLOAD(m_hInst))
			Warning("Could not unload dynamic library %s.  System Error: %s DynLib::unload", mName.c_str(), GetError().c_str());
	}

	//-----------------------------------------------------------------------
	void* DynLib::GetSymbol(const std::string& strName) const throw()
	{
		return (void*)DYNLIB_GETSYM(m_hInst, strName.c_str());
	}

	//-----------------------------------------------------------------------
	std::string DynLib::GetError(void)
	{
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
			);
		std::string ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree(lpMsgBuf);
		return ret;
#elif  (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
		return std::string(dlerror());
#elif PLATFORM_OS == PLATFORM_OS_MACOSX
		return std::string(mac_errorBundle());
#else
		return std::string("");
#endif
	}
}