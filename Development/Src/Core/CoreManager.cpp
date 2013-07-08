#include "CorePrivate.h"
#include "CoreManager.h"

namespace NGEngine {

	CoreManager::CoreManager()
		:rc(false),
		ec(false),
		iWindow(nullptr),
		iRender(nullptr),
		running(false)
	{}
}