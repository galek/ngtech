#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
// critical section

#if USE_PROFILER

// Critical Section
#if IS_OS_WINDOWS
typedef CRITICAL_SECTION ZCriticalSection_t;
ENGINE_INLINE char* ZGetCurrentDirectory(int bufLength, char *pszDest)
{
	return (char*)GetCurrentDirectoryA(bufLength, pszDest);
}

#elif IS_OS_LINUX
#include <pthread.h>
typedef pthread_mutex_t ZCriticalSection_t;
inline char* ZGetCurrentDirectory(int bufLength, char *pszDest)
{
	return getcwd(pszDest, bufLength);
}

#elif IS_OS_MACOSX
#import <CoreServices/CoreServices.h>
typedef MPCriticalRegionID ZCriticalSection_t;
inline char* ZGetCurrentDirectory(int bufLength, char *pszDest)
{
	return getcwd(pszDest, bufLength);
}
#endif


ENGINE_INLINE ZCriticalSection_t* NewCriticalSection()
{
#if IS_OS_LINUX
	ZCriticalSection_t *cs = new pthread_mutex_t;
	//(*cs) = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init (cs, NULL);
	return cs;
#elif IS_OS_MACOSX
	MPCriticalRegionID* criticalRegion = new MPCriticalRegionID;
	OSStatus err = MPCreateCriticalRegion (criticalRegion);
	if (err != 0)
	{
		delete criticalRegion;
		criticalRegion = NULL;
	}

	return criticalRegion;
#elif IS_OS_WINDOWS
	CRITICAL_SECTION *cs = new CRITICAL_SECTION;
	InitializeCriticalSection(cs);
	return cs;
#endif
}

ENGINE_INLINE void DestroyCriticalSection(ZCriticalSection_t *cs)
{
#if IS_OS_LINUX
	delete cs;
#elif IS_OS_MACOSX
	MPDeleteCriticalRegion(*cs);
#elif IS_OS_WINDOWS
	DeleteCriticalSection(cs);
	delete cs;
#endif
}

ENGINE_INLINE void LockCriticalSection(ZCriticalSection_t *cs)
{
#if IS_OS_LINUX
	pthread_mutex_lock( cs );
#elif IS_OS_MACOSX
	MPEnterCriticalRegion(*cs, kDurationForever);
#elif IS_OS_WINDOWS
	EnterCriticalSection(cs);
#endif
}

ENGINE_INLINE void UnLockCriticalSection(ZCriticalSection_t *cs)
{
#if IS_OS_LINUX
	pthread_mutex_unlock( cs );
#elif IS_OS_MACOSX
	MPExitCriticalRegion(*cs);
#elif IS_OS_WINDOWS
	LeaveCriticalSection(cs);
#endif 
}


CORE_API bool Zprofiler_enable();
CORE_API void Zprofiler_disable();
CORE_API void Zprofiler_start(const char *profile_name);
CORE_API void Zprofiler_end();
CORE_API void LogProfiler();

//defines

#define PROFILER_ENABLE Zprofiler_enable()
#define PROFILER_DISABLE Zprofiler_disable()
#define PROFILER_START(x) Zprofiler_start(QUOTE(x))
#define PROFILER_END() Zprofiler_end()
#define PROFILER_LOG() LogProfiler()

#else

#define PROFILER_LOG()

#define PROFILER_ENABLE
#define PROFILER_DISABLE
#define PROFILER_START(x)
#define PROFILER_END()
#endif