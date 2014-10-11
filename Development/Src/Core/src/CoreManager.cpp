#include "CorePrivate.h"
#include "MeshFormatXSMSH.h"

#include "SteamWorksMgr.h"
#include "StatsAndAchievements.h"

namespace NGTech {

	/**
	*/
	void DestroyAdditions();

	/**
	*/
	CoreManager::CoreManager()
		:paused(false), mIsEditor(false), iWindow(nullptr),
		iRender(nullptr), cvars(nullptr), log(nullptr),
		config(nullptr), vfs(nullptr), alSystem(nullptr),
		physSystem(nullptr), cache(nullptr), gui(nullptr),
		scene(nullptr), scripting(nullptr), mWatermarkTex(nullptr),
#ifdef USE_STEAMWORKS
		steamworks(nullptr),
#endif
		running(false)
	{
		SetCore(this);
#ifndef _ENGINE_DEBUG_
		if (!_InitAdditions())
			exit(0);
#else
		_InitAdditions();
#endif
	}

	/**
	*/
	CoreManager::~CoreManager()
	{
#ifdef USE_STEAMWORKS
		SAFE_DELETE(steamworks);
#endif
		DestroyAdditions();
	}

	/**
	*/
	void CoreManager::CorePreInit()
	{
		meshLoader = new MeshLoader();
		if (!meshLoader)
			Warning("[Init] MeshLoader Failed");
		{
			meshLoader->RegisterFormat(new MeshFormatXSMSH());
		}
	}
}