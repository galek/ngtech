#include "CorePrivate.h"
#include "CoreManager.h"

//***************************************************
#include "tbb/task_scheduler_init.h"
using namespace tbb;
//***************************************************
namespace NGEngine {

	CoreManager::CoreManager()
		:iWindow(nullptr),
		iRender(nullptr),
		running(false)	{
#ifdef _ENGINE_MT
			task_scheduler_init mThreads();
#endif
	}
}