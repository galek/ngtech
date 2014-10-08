#pragma once

#ifdef USE_STEAMWORKS
//**************************************
class ISteamUser;
class ISteamUserStats;
//**************************************
#include "steamworks/steam/steam_api.h"
#include "steamworks/steam/ISteamUserStats.h"
//**************************************

namespace NGTech
{
	/**
	*/
	class StatsAndAchievements
	{
	public:
		StatsAndAchievements();
		/**
		Добавляем пользователю ачивку
		*/
		bool SetAchievement(const char* API_NAME);
		/**
		Удаляем у пользователя ачивку
		*/
		bool DeleteAchievement(const char* API_NAME);
		/**
		Устанавливаем статистику
		*/
		void SetStat(const char* _name, int _value);
		/**
		Получаем статистику
		*/
		void GetStat(const char*_name, int &_value);
	private:
		// Steam UserStats interface
		ISteamUserStats *m_pSteamUserStats;

		// Did we get the stats from Steam?
		bool m_bRequestedStats;
		bool m_bStatsValid;

		// Should we store stats this frame?
		bool m_bStoreStats;
	};
}
#endif