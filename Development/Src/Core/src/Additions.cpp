#include "CorePrivate.h"


namespace NGTech {
	/**
	*/
	void ILSystemInit();
	/**
	*/
#ifdef USE_STEAMWORKS
	/**
	SteamWorks
	*/
	bool InitSteamWorks();
	void DestroySteamWorks();
#endif
#ifdef USE_OWN_MINIDUMP
	bool InitMiniDump();
#endif
	/**
	*/
	bool InitAdditions()
	{
#ifdef USE_STEAMWORKS
		if(!InitSteamWorks())
			return false;
#endif

#ifdef USE_OWN_MINIDUMP
		if (!InitMiniDump())
			return false;
#endif
		ILSystemInit();
		return true;
	}

	/**
	*/
	void DestroyAdditions(){
#ifdef USE_STEAMWORKS
		DestroySteamWorks();
#endif
	}
}