#include "CorePrivate.h"
#include "CoreManager.h"

//#include "..\Common\ThreadPool.h"

namespace VEGA {

	CoreManager::CoreManager()
		: //mTP(nullptr),
		iWindow(nullptr),
		iRender(nullptr),
		cvars(nullptr),
		log(nullptr),
		config(nullptr),
		vfs(nullptr),
		alSystem(nullptr),
		ilSystem(nullptr),
		physSystem(nullptr),
		cash(nullptr),
		gui(nullptr),
		scene(nullptr),
		running(false)	{
		//	mTP=new ThreadPool(4);
	}
}