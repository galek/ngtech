#include "CorePrivate.h"
//**************************************
#if (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
#include <sys/stat.h>
#elif  PLATFORM_OS == PLATFORM_OS_WINDOWS
#include <direct.h> //_mkdir
#endif
#include <stdlib.h>
#include <stdio.h>
//**************************************
#include "MeshFormatNGGFStatic.h"
#include "SkinnedMeshLoader.h"
#include "MeshFormatNGGFSkinned.h"
//**************************************
#include "SteamWorksMgr.h"
#include "StatsAndAchievements.h"
//**************************************

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
		debug(nullptr),
#ifdef USE_STEAMWORKS
		steamworks(nullptr),
#endif
		running(false)
	{
		SetCore(this);
		if (!_InitAdditions())
			exit(0);

		_preInit();
	}

	/**
	*/
	CoreManager::~CoreManager()
	{
		SAFE_DELETE(skinnedMeshLoader);
		SAFE_DELETE(meshLoader);
		SAFE_DELETE(debug);
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
		debug = new DebugLayer();
		meshLoader = new MeshLoader();
		meshLoader->RegisterFormat(new MeshFormatNGGFStatic());

		/**
		*/
		skinnedMeshLoader = new SkinnedMeshLoader();
		skinnedMeshLoader->RegisterFormat(new MeshFormatNGGFSkinned());
	}

	/**
	*/
	void CoreManager::_initUserDir(const char* dir) {
#if (PLATFORM_OS == PLATFORM_OS_LINUX) || (PLATFORM_OS == PLATFORM_OS_ANDROID)
		//with read/write/search permissions for owner and group, and with read/search permissions for others.
		mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#elif PLATFORM_OS == PLATFORM_OS_WINDOWS
		_mkdir(dir);
#endif
	}
}