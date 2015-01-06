//***************************************************
#include "EnginePrivate.h"
//***************************************************
#include "../inc/WindowSystem_GLFW.h"
//***************************************************
#if  PLATFORM_OS != PLATFORM_OS_WINDOWS
#include <sys/time.h>
#endif
namespace NGTech {

	/**
	*/
	unsigned long WindowSystemGLFW::GetTicks()
	{
#if  PLATFORM_OS == PLATFORM_OS_WINDOWS
		ulTicks = GetTickCount();
		return ulTicks;
#else
		struct timeval now;
		gettimeofday(&now, NULL);
		return (now.tv_sec) * 1000 + (now.tv_usec) / 1000;
#endif
	}
}