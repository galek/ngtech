#include "CorePrivate.h"

namespace NGTech 
{
	/**
	*/
	bool DemoStartCheck();

	/**
	*/
	void ILSystemInit();

	/**
	*/
	bool EnableProfiler();

	/**
	*/
	void DisableProfiler();

	/**
	*/
#ifdef USE_STEAMWORKS
	/**
	SteamWorks
	*/
	void DestroySteamWorks();
#endif

#ifdef USE_OWN_MINIDUMP
	/**
	*/
	bool InitMiniDump();
#endif
	/**
	*/
	bool InitAdditions()
	{
		if (!EnableProfiler())
			return false;

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
		DisableProfiler();

#ifdef USE_STEAMWORKS
		DestroySteamWorks();
#endif
	}

	static Remotery *rmt;

	/**
	*/
	bool EnableProfiler()
	{
		if (RMT_ERROR_NONE != rmt_CreateGlobalInstance(&rmt)) {
			return false;
		}
		return true;
	}

	/**
	*/
	void DisableProfiler()
	{
		rmt_DestroyGlobalInstance(rmt);
	}
}