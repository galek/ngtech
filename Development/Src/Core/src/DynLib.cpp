#include "CorePrivate.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#define ENGINE_PLATFORM_LINUX 0
#define ENGINE_PLATFORM_WIN32 1
#define ENGINE_PLATFORM_APPLE 2
#define ENGINE_PLATFORM ENGINE_PLATFORM_WIN32

namespace NGTech{
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
#if ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
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
#if ENGINE_PLATFORM == ENGINE_PLATFORM_WIN32
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
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
		return string(dlerror());
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_APPLE
		return string(mac_errorBundle());
#else
		return string("");
#endif
	}
}