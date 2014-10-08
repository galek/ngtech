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
	void DestroySteamWorks();
#endif
#ifdef USE_OWN_MINIDUMP
	bool InitMiniDump();
#endif
	/**
	*/
	bool InitAdditions()
	{
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