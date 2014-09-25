#include "CorePrivate.h"
#include "CoreManager.h"

namespace NGTech {
	/**
	*/
	void DestroyAdditions();
	bool InitAdditions();
	/**
	*/
	CoreManager::CoreManager()
		:paused(false),
		mIsEditor(false),
		iWindow(nullptr),
		iRender(nullptr),
		cvars(nullptr),
		log(nullptr),
		config(nullptr),
		vfs(nullptr),
		alSystem(nullptr),
		physSystem(nullptr),
		cache(nullptr),
		gui(nullptr),
		scene(nullptr),
		scripting(nullptr),
		mWatermarkTex(nullptr),
		running(false)
	{
		SetCore(this);
#ifndef _ENGINE_DEBUG_
		if (!InitAdditions())
			exit(0);
#else
		InitAdditions();
#endif
	}
	/**
	*/
	CoreManager::~CoreManager(){
		DestroyAdditions();
	}
}