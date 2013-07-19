#include "CorePrivate.h"
#include "CoreManager.h"

#include "..\Common\ThreadPool.h"

namespace VEGA {

	CoreManager::CoreManager()
		: mTP(nullptr),
		iWindow(nullptr),
		iRender(nullptr),
		running(false)	{
			mTP=new ThreadPool(4);
	}
}