#include "CorePrivate.h"
#include "CoreManager.h"

namespace NGTech {
	/**
	*/
	CoreManager::CoreManager()
		:mIsEditor(false),
		iWindow(nullptr),
		iRender(nullptr),
		cvars(nullptr),
		log(nullptr),
		config(nullptr),
		vfs(nullptr),
		alSystem(nullptr),
		ilSystem(nullptr),
		physSystem(nullptr),
		cache(nullptr),
		gui(nullptr),
		scene(nullptr),
		scripting(nullptr),
		running(false)
	{
		SetCore(this);
	}
	/**
	*/
}