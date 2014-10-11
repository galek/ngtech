#include "CorePrivate.h"
//***************************************************************************
#include "Log.h"
#include <tchar.h>
#include <stdarg.h>
//***************************************************************************

#ifdef USE_PROFILER

using namespace std;
using namespace NGTech;

void TimerInit();
double	startHighResolutionTimer(void);

void ProfilerPrintf(const char *fmt, ...){

	char           msg[8000];

	va_list         argptr;
	va_start(argptr, fmt);
	_vstprintf(msg, fmt, argptr);
	va_end(argptr);

	if (std::string(msg).empty())
		return;

	Log::profiler(msg);
}


#if IS_OS_WINDOWS
// Create A Structure For The Timer Information
struct
{
	__int64			frequency;									// Timer Frequency
	float			resolution;									// Timer Resolution
	unsigned long	mm_timer_start;								// Multimedia Timer Start Value
	unsigned long	mm_timer_elapsed;							// Multimedia Timer Elapsed Time
	bool			performance_timer;							// Using The Performance Timer?
	__int64			performance_timer_start;					// Performance Timer Start Value
	__int64			performance_timer_elapsed;					// Performance Timer Elapsed Time
} timer;														// Structure Is Named timer

#endif  //  IS_OS_WINDOWS


typedef struct stGenProfilerData
{
	double			totalTime;
	double			averageTime;
	double			minTime;
	double			maxTime;
	double			lastTime;				// Time of the previous passage
	double			elapsedTime;			// Elapsed Time
	unsigned long	nbCalls;				// Numbers of calls
	char			szBunchCodeName[1024];	// temporary.
} tdstGenProfilerData;

//  Hold the call stack
typedef std::vector<tdstGenProfilerData> tdCallStackType;

//	Hold the sequence of profiler_starts
std::map<std::string, tdstGenProfilerData> mapProfilerGraph;

// Hold profiler data vector in function of the thread
map<unsigned long, tdCallStackType> mapCallsByThread;

// Critical section
ZCriticalSection_t	*gProfilerCriticalSection;

//
// Activate the profiler
//
CORE_API bool Zprofiler_enable()
{
	// Initialize the timer
	TimerInit();

	// Create the mutex
	gProfilerCriticalSection = NewCriticalSection();

	return true;
}

//
// Deactivate the profiler
//
CORE_API void Zprofiler_disable()
{
	// Clear maps
	mapCallsByThread.clear();
	mapProfilerGraph.clear();
	mapCallsByThread.clear();

	// Delete the mutex
	DestroyCriticalSection(gProfilerCriticalSection);
}
#if IS_OS_MACOSX
unsigned long GetCurrentThreadId() { return 0; }
#elif IS_OS_LINUX
unsigned long GetCurrentThreadId() { return 0; }
#endif

//
// Start the profiling of a bunch of code
//
CORE_API void Zprofiler_start(const char *profile_name)
{
	LockCriticalSection(gProfilerCriticalSection);

	unsigned long ulThreadId = GetCurrentThreadId();

	// Add the profile name in the callstack vector
	tdstGenProfilerData	GenProfilerData;
	memset(&GenProfilerData, 0, sizeof(GenProfilerData));
	GenProfilerData.lastTime = startHighResolutionTimer();
	GenProfilerData.minTime = 0xFFFFFFFF;

	// Find or add callstack
	tdCallStackType TmpCallStack;
	map<unsigned long, tdCallStackType>::iterator IterCallsByThreadMap = mapCallsByThread.find(ulThreadId);
	if (IterCallsByThreadMap == mapCallsByThread.end())
	{
		// Not found. So insert the new pair
		mapCallsByThread.insert(std::make_pair(ulThreadId, TmpCallStack));
		IterCallsByThreadMap = mapCallsByThread.find(ulThreadId);
	}

	// It's the first element of the vector
	if ((*IterCallsByThreadMap).second.empty())
	{
		GenProfilerData.nbCalls = 1;
		sprintf(GenProfilerData.szBunchCodeName, "%s%d%s%s", _NAME_SEPARATOR_, (int)ulThreadId, _THREADID_NAME_SEPARATOR_, profile_name);
		(*IterCallsByThreadMap).second.push_back(GenProfilerData);
	}
	// It's not the first element of the vector
	else
	{
		// Update the number of calls
		GenProfilerData.nbCalls++;

		// We need to construct the string with the previous value of the
		// profile_start
		char *previousString = (*IterCallsByThreadMap).second[(*IterCallsByThreadMap).second.size() - 1].szBunchCodeName;

		// Add the current profile start string
		strcpy(GenProfilerData.szBunchCodeName, previousString);
		strcat(GenProfilerData.szBunchCodeName, _NAME_SEPARATOR_);
		strcat(GenProfilerData.szBunchCodeName, profile_name);

		// Push it
		(*IterCallsByThreadMap).second.push_back(GenProfilerData);
	}

	UnLockCriticalSection(gProfilerCriticalSection);
}

//
// Stop the profiling of a bunch of code
//
CORE_API void Zprofiler_end()
{
	unsigned long ulThreadId = GetCurrentThreadId();

	// Retrieve the right entry in function of the threadId
	map<unsigned long, tdCallStackType>::iterator IterCallsByThreadMap = mapCallsByThread.find(ulThreadId);

	// Check if vector is empty
	if ((*IterCallsByThreadMap).second.empty())
	{
		ProfilerPrintf("There is an error in the vector CallStack !!!\n\n");
		return;
	}

	LockCriticalSection(gProfilerCriticalSection);

	// Retrieve the last element from the callstack vector
	tdstGenProfilerData	GenProfilerData;
	GenProfilerData = (*IterCallsByThreadMap).second[(*IterCallsByThreadMap).second.size() - 1];

	// Compute elapsed time
	GenProfilerData.elapsedTime += startHighResolutionTimer() - GenProfilerData.lastTime;
	GenProfilerData.totalTime += GenProfilerData.elapsedTime;

	// Find if this entry exists in the map
	std::map<std::string, tdstGenProfilerData>::iterator	IterMap;
	IterMap = mapProfilerGraph.find(GenProfilerData.szBunchCodeName);
	if (IterMap != mapProfilerGraph.end())
	{
		(*IterMap).second.nbCalls++;

		// Retrieve time information to compute min and max time
		double minTime = (*IterMap).second.minTime;
		double maxTime = (*IterMap).second.maxTime;
		//double totalTime	= (*IterMap).second.totalTime;

		if (GenProfilerData.elapsedTime < minTime)
		{
			(*IterMap).second.minTime = GenProfilerData.elapsedTime;
		}

		if (GenProfilerData.elapsedTime > maxTime)
		{
			(*IterMap).second.maxTime = GenProfilerData.elapsedTime;
		}

		// Compute Total Time
		(*IterMap).second.totalTime += GenProfilerData.elapsedTime;
		// Compute Average Time
		(*IterMap).second.averageTime = (*IterMap).second.totalTime / (*IterMap).second.nbCalls;

	}
	else
	{
		GenProfilerData.minTime = GenProfilerData.elapsedTime;
		GenProfilerData.maxTime = GenProfilerData.elapsedTime;

		// Compute Average Time
		GenProfilerData.averageTime = GenProfilerData.totalTime / GenProfilerData.nbCalls;

		// Insert this part of the call stack into the Progiler Graph
		mapProfilerGraph.insert(std::make_pair(GenProfilerData.szBunchCodeName, GenProfilerData));
	}

	// Now, pop back the GenProfilerData from the vector callstack
	(*IterCallsByThreadMap).second.pop_back();

	UnLockCriticalSection(gProfilerCriticalSection);
}

//
// Dump all data in a file
//

bool MyDataSortPredicate(const tdstGenProfilerData un, const tdstGenProfilerData deux)
{
	return un.szBunchCodeName > deux.szBunchCodeName;
}

CORE_API void LogProfiler()
{

	// Thread Id String
	char szThreadId[16];
	char textLine[1024];
	char *tmpString;

	long i;
	//long nbTreadIds	= 0;
	long size = 0;

	// Map for calls
	std::map<std::string, tdstGenProfilerData> mapCalls;
	std::map<std::string, tdstGenProfilerData>::iterator IterMapCalls;
	mapCalls.clear();

	// Map for Thread Ids
	std::map<std::string, int> ThreadIdsCount;
	std::map<std::string, int>::iterator IterThreadIdsCount;
	ThreadIdsCount.clear();

	// Vector for callstack
	vector<tdstGenProfilerData> tmpCallStack;
	vector<tdstGenProfilerData>::iterator IterTmpCallStack;
	tmpCallStack.clear();

	// Copy map data into a vector in the aim to sort it
	std::map<std::string, tdstGenProfilerData>::iterator	IterMap;
	for (IterMap = mapProfilerGraph.begin(); IterMap != mapProfilerGraph.end(); ++IterMap)
	{
		strcpy((*IterMap).second.szBunchCodeName, (*IterMap).first.c_str());
		tmpCallStack.push_back((*IterMap).second);
	}

	// Sort the vector
	std::sort(tmpCallStack.begin(), tmpCallStack.end(), MyDataSortPredicate);

	// Create a map with thread Ids
	for (IterTmpCallStack = tmpCallStack.begin(); IterTmpCallStack != tmpCallStack.end(); ++IterTmpCallStack)
	{
		//// DEBUG
		//fprintf(DumpFile, "%s\n", (*IterTmpCallStack).szBunchCodeName );
		//// DEBUG

		tmpString = strstr((*IterTmpCallStack).szBunchCodeName, _THREADID_NAME_SEPARATOR_);
		size = (long)(tmpString - (*IterTmpCallStack).szBunchCodeName);
		strncpy(szThreadId, (*IterTmpCallStack).szBunchCodeName + 1, size - 1); szThreadId[size - 1] = 0;
		ThreadIdsCount[szThreadId]++;
	}

	// Retrieve the number of thread ids
	//unsigned long nbThreadIds	= mapProfilerGraph.size();

	IterThreadIdsCount = ThreadIdsCount.begin();
	for (unsigned long nbThread = 0; nbThread < ThreadIdsCount.size(); nbThread++)
	{
		sprintf(szThreadId, "%s", IterThreadIdsCount->first.c_str());

		ProfilerPrintf("CALLSTACK of Thread %s\n", szThreadId);
		ProfilerPrintf("_______________________________________________________________________________________\n");
		ProfilerPrintf("| Total time   | Avg Time     |  Min time    |  Max time    | Calls  | Section\n");
		ProfilerPrintf("_______________________________________________________________________________________\n");

		long nbSeparator = 0;
		for (IterTmpCallStack = tmpCallStack.begin(); IterTmpCallStack != tmpCallStack.end(); ++IterTmpCallStack)
		{
			tmpString = (*IterTmpCallStack).szBunchCodeName + 1;

			if (strstr(tmpString, szThreadId))
			{
				// Count the number of separator in the string
				nbSeparator = 0;
				while (*tmpString)
				{
					if (*tmpString++ == '|')
					{
						nbSeparator++;
					}
				}

				// Get times and fill in the dislpay string
				sprintf(textLine, "| %12.4f | %12.4f | %12.4f | %12.4f |%6d  | ",
					(*IterTmpCallStack).totalTime,
					(*IterTmpCallStack).averageTime,
					(*IterTmpCallStack).minTime,
					(*IterTmpCallStack).maxTime,
					(int)(*IterTmpCallStack).nbCalls);

				// Get the last start_profile_name in the string
				tmpString = strrchr((*IterTmpCallStack).szBunchCodeName, '|') + 1;

				IterMapCalls = mapCalls.find(tmpString);
				if (IterMapCalls != mapCalls.end())
				{
					double minTime = (*IterMapCalls).second.minTime;
					double maxTime = (*IterMapCalls).second.maxTime;

					if ((*IterTmpCallStack).minTime < minTime)
					{
						(*IterMapCalls).second.minTime = (*IterTmpCallStack).minTime;
					}
					if ((*IterTmpCallStack).maxTime > maxTime)
					{
						(*IterMapCalls).second.maxTime = (*IterTmpCallStack).maxTime;
					}
					(*IterMapCalls).second.totalTime += (*IterTmpCallStack).totalTime;
					(*IterMapCalls).second.nbCalls += (*IterTmpCallStack).nbCalls;
					(*IterMapCalls).second.averageTime = (*IterMapCalls).second.totalTime / (*IterMapCalls).second.nbCalls;
				}

				else
				{
					tdstGenProfilerData tgt;
					if (strstr(tmpString, szThreadId))
					{
						strcpy(tgt.szBunchCodeName, tmpString);
					}
					else
					{
						sprintf(tgt.szBunchCodeName, "%s%s%s",
							szThreadId,
							_THREADID_NAME_SEPARATOR_,
							tmpString);
					}

					tgt.minTime = (*IterTmpCallStack).minTime;
					tgt.maxTime = (*IterTmpCallStack).maxTime;
					tgt.totalTime = (*IterTmpCallStack).totalTime;
					tgt.averageTime = (*IterTmpCallStack).averageTime;
					tgt.elapsedTime = (*IterTmpCallStack).elapsedTime;
					tgt.lastTime = (*IterTmpCallStack).lastTime;
					tgt.nbCalls = (*IterTmpCallStack).nbCalls;

					mapCalls.insert(std::make_pair(tmpString, tgt));
				}


				// Copy white space in the string to format the display
				// in function of the hierarchy
				for (i = 0; i < nbSeparator; i++) strcat(textLine, "  ");

				// Remove the thred if from the string
				if (strstr(tmpString, _THREADID_NAME_SEPARATOR_))
				{
					tmpString += strlen(szThreadId) + 1;
				}

				// Display the name of the bunch code profiled
				ProfilerPrintf("%s%s\n", textLine, tmpString);
			}
		}
		ProfilerPrintf("_______________________________________________________________________________________\n\n");
		++IterThreadIdsCount;
	}
	ProfilerPrintf("\n\n");

	//
	//	DUMP CALLS
	//
	IterThreadIdsCount = ThreadIdsCount.begin();
	for (unsigned long nbThread = 0; nbThread < ThreadIdsCount.size(); nbThread++)
	{
		sprintf(szThreadId, "%s", IterThreadIdsCount->first.c_str());

		ProfilerPrintf("DUMP of Thread %s\n", szThreadId);
		ProfilerPrintf("_______________________________________________________________________________________\n");
		ProfilerPrintf("| Total time   | Avg Time     |  Min time    |  Max time    | Calls  | Section\n");
		ProfilerPrintf("_______________________________________________________________________________________\n");

		for (IterMapCalls = mapCalls.begin(); IterMapCalls != mapCalls.end(); ++IterMapCalls)
		{
			tmpString = (*IterMapCalls).second.szBunchCodeName;
			if (strstr(tmpString, szThreadId))
			{
				ProfilerPrintf("| %12.4f | %12.4f | %12.4f | %12.4f | %6d | %s\n",
					(*IterMapCalls).second.totalTime,
					(*IterMapCalls).second.averageTime,
					(*IterMapCalls).second.minTime,
					(*IterMapCalls).second.maxTime,
					(int)(*IterMapCalls).second.nbCalls,
					(*IterMapCalls).second.szBunchCodeName + strlen(szThreadId) + 1);
			}
		}
		ProfilerPrintf("_______________________________________________________________________________________\n\n");
		++IterThreadIdsCount;
	}

}

////
////	Gestion des timers
////
#if IS_OS_WINDOWS

// Initialize Our Timer (Get It Ready)
void TimerInit()
{
	memset(&timer, 0, sizeof(timer));

	// Check to see if a performance counter is available
	// If one is available the timer frequency will be updated
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&timer.frequency))
	{
		// No performace counter available
		timer.performance_timer = false;					// Set performance timer to false
		timer.mm_timer_start = timeGetTime();			// Use timeGetTime() to get current time
		timer.resolution = 1.0f / 1000.0f;				// Set our timer resolution to .001f
		timer.frequency = 1000;						// Set our timer frequency to 1000
		timer.mm_timer_elapsed = timer.mm_timer_start;		// Set the elapsed time to the current time
	}
	else
	{
		// Performance counter is available, use it instead of the multimedia timer
		// Get the current time and store it in performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *)&timer.performance_timer_start);
		timer.performance_timer = true;				// Set performance timer to true

		// Calculate the timer resolution using the timer frequency
		timer.resolution = (float)(((double)1.0f) / ((double)timer.frequency));

		// Set the elapsed time to the current time
		timer.performance_timer_elapsed = timer.performance_timer_start;
	}
}

// platform specific get hires times...
double startHighResolutionTimer()
{
	__int64 time;

	// Are we using the performance timer?
	if (timer.performance_timer)
	{
		// Grab the current performance time
		QueryPerformanceCounter((LARGE_INTEGER *)&time);

		// Return the current time minus the start time multiplied
		// by the resolution and 1000 (To Get MS)
		return ((double)(time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		// Return the current time minus the start time multiplied
		// by the resolution and 1000 (To Get MS)
		return((double)(timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

#elif IS_OS_MACOSX

// Initialize Our Timer (Get It Ready)
void TimerInit()
{
}

double startHighResolutionTimer()
{
	UnsignedWide t;
	Microseconds(&t);
	double ms = double(t.hi * 1000LU);
	ms += double(t.lo / 1000LU);//*0.001;


	return ms;
}

#else

// Initialize Our Timer (Get It Ready)
void TimerInit()
{
}

double startHighResolutionTimer()
{
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux


	double ms = double(ts.tv_sec * 1000LU);
	ms += double(ts.tv_nsec / 1000LU)*0.001;


	return ms;
}
#endif
#endif