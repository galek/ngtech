#include "CorePrivate.h"
#include "CoreManager.h"

#include "..\Common\ThreadPool.h"

namespace VEGA {
	void InitMiniDumper();

	CoreManager::CoreManager()
		: mTP(nullptr),
		iWindow(nullptr),
		iRender(nullptr),
		running(false)	{
			InitMiniDumper();
			mTP=new ThreadPool(4);
	}
}