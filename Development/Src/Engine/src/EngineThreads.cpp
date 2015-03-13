/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    EngineThreads.cpp
* Desc:    Engine Threading impl.
* Version: 1.01
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#include "EnginePrivate.h"
//***************************************************
#include "Engine.h"
#include "EngineThreads.h"
#include "Utils.h"
#include "../../Core/inc/systeminfo.h"
#include "Scene.h"
//***************************************************

namespace NGTech
{
	/*
	 */
#define ENGINE_THREADS_NUM_JOBS	128


	/******************************************************************************\
	*
	* Task-Based Threading
	*
	\******************************************************************************/

	/**
	*/
	class EngineJobThread : public Thread {

	public:

		EngineJobThread(int num);
		virtual ~EngineJobThread();

		void clearJobs();
		void addJob(EngineJobBase *job);

	protected:

		virtual void process();

	private:

		int num;

		Vector<EngineJobBase*> jobs;
	};

	/**
	*/
	EngineJobThread::EngineJobThread(int num) : num(num) {
		jobs.allocate(ENGINE_THREADS_NUM_JOBS);
	}

	/**
	*/
	EngineJobThread::~EngineJobThread() {

	}

	/**
	*/
	void EngineJobThread::clearJobs() {
		jobs.clear();
	}

	void EngineJobThread::addJob(EngineJobBase *job) {
		jobs.append(job);
	}

	/**
	*/
	void EngineJobThread::process() {

#if PLATFORM_OS == PLATFORM_OS_WINDOWS
		SetThreadIdealProcessor(thread, num + 1);
#endif

		wait();

		while (isRunning()) {

			// run jobs
			for (int i = 0; i < jobs.size(); i++) {
				jobs[i]->process();
			}

			wait();
		}
	}

	/******************************************************************************\
	*
	* EngineThreads
	*
	\******************************************************************************/

	/**
	*/
	EngineThreads::EngineThreads() :job_lock(0) {
		// create job threads
		int num_jobs = SystemInfo::getCPUCount() - 1;
		if (num_jobs < 1) num_jobs = 1;
		for (int i = 0; i < num_jobs; i++) {
			EngineJobThread *thread = new EngineJobThread(i);
			job_threads.append(thread);
			if (thread->run() == 0) {
				Error("Engine::init(): can't run job thread\n", true);
			}
		}
	}

	EngineThreads::~EngineThreads() {

		// delete job threads
		for (int i = 0; i < job_threads.size(); i++) {
			delete job_threads[i];
		}

		// delete tasks
		for (int i = 0; i < tasks.size(); i++) {
			delete tasks[i];
		}
	}

	
	/*
	 */
	int EngineThreads::getNumJobs() const {
		return job_threads.size();
	}

	/*
	*/
	int EngineThreads::runJobs(void *jobs, int stride, int num) {

		AtomicLock atomic(&job_lock);

		// find free task
		int id = -1;
		EngineTask *task = NULL;
		for (int i = 0; i < tasks.size(); i++) {
			if (tasks[i]->jobs.size()) continue;
			if (tasks[i]->threads.size()) continue;
			task = tasks[i];
			id = i;
			break;
		}
		if (task == NULL) {
			task = new EngineTask();
			tasks.append(task);
			id = tasks.size() - 1;
		}

		// empty task
		if (num == 0) return id;

		// sort threads
		quickSort(job_threads.begin(), job_threads.end());

		// find free threads
		for (int i = 0; i < job_threads.size(); i++) {
			task->threads.append(job_threads[i]);
			job_threads.remove(i--);
			if (task->threads.size() >= num) break;
		}

		// no such free threads
		if (task->threads.size() == 0) {
			unsigned char *j = static_cast<unsigned char*>(jobs);
			for (int i = 0; i < num; i++) {
				EngineJobBase *job = reinterpret_cast<EngineJobBase*>(j);
				job->process();
				j += stride;
			}
			return id;
		}

		// manage threads
		int thread = 0;
		task->jobs.allocate(num);
		unsigned char *j = static_cast<unsigned char*>(jobs);
		for (int i = 0; i < num; i++) {
			EngineJobBase *job = reinterpret_cast<EngineJobBase*>(j);
			task->jobs.append(job);
			task->threads[thread++]->addJob(job);
			if (thread >= task->threads.size()) thread = 0;
			j += stride;
		}

		// run threads
		for (int i = 0; i < task->threads.size(); i++) {
			if (task->threads[i]->signal() == 0) {
				Error("EngineThreads::runJobs(): can't run job thread\n", true);
			}
		}

		return id;
	}

	void EngineThreads::waitJobs(int id) {

		// wait job
		while (1) {
			int is_waiting = 1;
			for (int i = 0; i < tasks[id]->threads.size(); i++) {
				if (tasks[id]->threads[i]->isWaiting() == 0) {
					is_waiting = 0;
					break;
				}
			}
			if (is_waiting) break;
		}

		AtomicLock atomic(&job_lock);

		// clear job
		for (int i = 0; i < tasks[id]->threads.size(); i++) {
			job_threads.append(tasks[id]->threads[i]);
			tasks[id]->threads[i]->clearJobs();
		}
		tasks[id]->jobs.clear();
		tasks[id]->threads.clear();
	}

	void EngineThreads::stopJobs() {

		AtomicLock atomic(&job_lock);

		for (int i = 0; i < tasks.size(); i++) {
			for (int j = 0; j < tasks[i]->threads.size(); j++) {
				tasks[i]->threads[j]->terminate();
			}
		}
		for (int i = 0; i < job_threads.size(); i++) {
			job_threads[i]->terminate();
		}
	}
}