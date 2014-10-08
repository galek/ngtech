#pragma once

#ifdef USE_STEAMWORKS
namespace NGTech
{
	/**
	*/
	class StatsAndAchievements;

	/**
	*/
	struct SteamWorksMgr
	{
		StatsAndAchievements* stats;
	};
}
#endif