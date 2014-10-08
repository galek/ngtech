#pragma once


namespace NGTech {

	/**
	*/
	void CORE_API API_SteamWorks_SetAchievement(const char* _name);

	/**
	*/
	void CORE_API API_SteamWorks_DeleteAchievement(const char* _name);

	/**
	*/
	void CORE_API API_SteamWorks_SetStat(const char*_name, int _value);

	/**
	*/
	void CORE_API API_SteamWorks_GetStat(const char* _name, int &_value);
}