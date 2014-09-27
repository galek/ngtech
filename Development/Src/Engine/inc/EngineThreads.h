#ifndef __ENGINE_THREADS_H__
#define __ENGINE_THREADS_H__

#include "Thread.h"
#include "Engine.h"
#include "Callback.h"
#include "Vector.h"//added

namespace NGTech
{
	/*
	 */
	class SoundThread;
	class FileSystemThread;
	class EngineJobThread;

	/*
	 */
	class EngineJobBase {
	public:
		EngineJobBase() { }
		virtual ~EngineJobBase() { }

		virtual void process() = 0;
	};

	/*
	 */
	class EngineJobCallback : public EngineJobBase {
	public:
		EngineJobCallback() : callback(0) { }
		ENGINE_INLINE virtual ~EngineJobCallback() { delete callback; }

		ENGINE_INLINE void setCallback(CallbackBase *c) { callback = c; }

		ENGINE_INLINE virtual void process() { if (callback) callback->run(); }

	private:

		CallbackBase *callback;
	};

	/*
	 */
	class EngineThreads {
	public:
		EngineThreads();
		virtual ~EngineThreads();
				
		// engine jobs
		int getNumJobs() const;
		int runJobs(void *jobs, int stride, int num);
		void waitJobs(int id);
		void stopJobs();

	private:
		volatile int job_lock;					// job lock
		Vector<EngineJobThread*> job_threads;	// job threads

		struct EngineTask {
			Vector<EngineJobBase*> jobs;		// assigned jobs
			Vector<EngineJobThread*> threads;	// assigned threads
		};

		Vector<EngineTask*> tasks;				// task list
	};
}
#endif /* __ENGINE_THREADS_H__ */