#include "CorePrivate.h"

#ifdef USE_STEAMWORKS
//**************************************
#include "steamworks/steam/steam_api.h"
#include "steamworks/steam/ISteamUserStats.h"
//**************************************
#include "StatsAndAchievements.h"
//**************************************

namespace NGTech
{
	/**
	*/
	StatsAndAchievements::StatsAndAchievements()
		:m_pSteamUserStats(NULL)
	{
		m_pSteamUserStats = SteamUserStats();

		m_bRequestedStats = false;
		m_bStatsValid = false;
		m_bStoreStats = false;
	}

	/**
	*/
	bool StatsAndAchievements::SetAchievement(const char*_ach)
	{
		SteamUserStats()->SetAchievement(_ach);
		return SteamUserStats()->StoreStats();
	}

	/**
	*/
	bool StatsAndAchievements::DeleteAchievement(const char*_ach)
	{
		SteamUserStats()->ClearAchievement(_ach);
		return SteamUserStats()->StoreStats();
	}

	/**
	*/
	void StatsAndAchievements::SetStat(const char*_name, int _value)
	{
		if (m_bStoreStats)
		{
			m_pSteamUserStats->SetStat(_name, _value);
			// If this failed, we never sent anything to the server, try
			// again later.
			m_bStoreStats = !m_pSteamUserStats->StoreStats();
		}
	}

	/**
	*/
	void StatsAndAchievements::GetStat(const char*_name, int &_value)
	{
		if (m_bStoreStats)
		{
			m_pSteamUserStats->GetStat(_name, &_value);
			// If this failed, we never sent anything to the server, try
			// again later.
			m_bStoreStats = !m_pSteamUserStats->StoreStats();
		}
	}

	/**
	*/
}
#endif