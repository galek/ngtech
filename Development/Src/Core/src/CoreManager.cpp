#include "CorePrivate.h"
#include "MeshFormatXSMSH.h"

#include "SteamWorksMgr.h"
#include "StatsAndAchievements.h"


#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

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
		_preInit();
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
	void CoreManager::_preInit()
	{
		_initUserDir();
		meshLoader = new MeshLoader();
		if (!meshLoader)
			Warning("[Init] MeshLoader Failed");
		{
			meshLoader->RegisterFormat(new MeshFormatXSMSH());
		}
	}
	
	void CoreManager::_initUserDir(const char* dir)	{
		_mkdir(dir);
	}
}