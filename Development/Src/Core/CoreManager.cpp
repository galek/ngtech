#include "CorePrivate.h"
#include "CoreManager.h"

namespace VEGA {

	CoreManager::CoreManager()
		:iWindow(nullptr),
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
		running(false)	{
	}
}