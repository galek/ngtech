#include "CorePrivate.h"

#include "CoreManager.h"

#include "SteamWorksMgr.h"
#include "StatsAndAchievements.h"

namespace NGTech {

	/**
	*/
	CORE_API void API_SteamWorks_SetAchievement(const char* _name)
	{
#ifdef USE_STEAMWORKS
		GetSteamworksStats()->stats->SetAchievement(_name);
#else
		LogPrintf("Function %s,is not implemented.For using you must define 'USE_STEAMWORKS'", __FUNCTION__);
#endif
	}

	/**
	*/
	CORE_API void API_SteamWorks_DeleteAchievement(const char* _name)
	{
#ifdef USE_STEAMWORKS
		GetSteamworksStats()->stats->DeleteAchievement(_name);
#else
		LogPrintf("Function %s,is not implemented.For using you must define 'USE_STEAMWORKS'",__FUNCTION__);
#endif
	}

	/**
	*/
	CORE_API void API_SteamWorks_SetStat(const char*_name, int _value)
	{
#ifdef USE_STEAMWORKS
		GetSteamworksStats()->stats->SetStat(_name, _value);
#else
		LogPrintf("Function %s,is not implemented.For using you must define 'USE_STEAMWORKS'", __FUNCTION__);
#endif
	}

	/**
	*/
	CORE_API void API_SteamWorks_GetStat(const char* _name, int &_value)
	{
#ifdef USE_STEAMWORKS
		GetSteamworksStats()->stats->GetStat(_name, _value);
#else
		LogPrintf("Function %s,is not implemented.For using you must define 'USE_STEAMWORKS'", __FUNCTION__);
#endif
	}
}