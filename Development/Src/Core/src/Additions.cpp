#include "CorePrivate.h"


namespace NGTech 
{

	/**
	*/
	void DestroyAdditions();
	bool InitAdditions();

	/**
	SteamWorks
	*/
#ifdef USE_STEAMWORKS
	struct SteamWorksMgr* InitSteamWorks();
#endif

	/**
	*/
	bool CoreManager::_InitAdditions()
	{
#ifdef USE_STEAMWORKS
		if (!InitSteamWorks())
			return false;
#endif
		return InitAdditions();
	}
}