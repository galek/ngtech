#include "CorePrivate.h"

#include "libProfiler.h"

namespace NGTech {
	/**
	*/
	bool DemoStartCheck();
	/**
	*/
	void ILSystemInit();
	/**
	*/
#ifdef USE_STEAMWORKS
	/**
	SteamWorks
	*/
	void DestroySteamWorks();
#endif
#ifdef USE_OWN_MINIDUMP
	bool InitMiniDump();
#endif
	/**
	*/
	bool InitAdditions()
	{
		PROFILER_ENABLE;
#ifndef _ENGINE_DEBUG_
#ifdef USE_OWN_MINIDUMP
		if (!InitMiniDump())
			return false;
#endif
		/*	if (!DemoStartCheck())
				return false;*/
#endif	
		if (!DemoStartCheck())
			return false;
		ILSystemInit();

		return true;
	}

	/**
	*/
	void DestroyAdditions()
	{
		PROFILER_DISABLE;

#ifdef USE_STEAMWORKS
		DestroySteamWorks();
#endif
	}
}