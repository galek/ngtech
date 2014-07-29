#include "CorePrivate.h"

#ifdef _LINUX
#include <unistd.h>
#endif


#include "Thread.h"

namespace NGTech
{
	/*
	*/
	Thread::Thread() {
#ifdef _WIN32
		thread = NULL;
		event = CreateEvent(NULL, 0, 0, NULL);
#else
		thread = 0;
		pthread_cond_init(&cond, NULL);
		pthread_mutex_init(&mutex, NULL);
#endif
		running = 0;
		waiting = 0;
	}

	Thread::~Thread() {
		signal();
		terminate();
#ifdef _WIN32
		CloseHandle(event);
#else
		pthread_cond_destroy(&cond);
		pthread_mutex_destroy(&mutex);
#endif
	}

	/*
	 */
#ifdef _WIN32
	DWORD WINAPI Thread::thread_handler(LPVOID data) {
		Thread *thread = static_cast<Thread*>(data);
		if (thread->running) thread->process();
		thread->running = 0;
		return 0;
	}
#else
	void *Thread::thread_handler(void *data) {
		Thread *thread = static_cast<Thread*>(data);
#ifdef _LINUX
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
#endif
		if(thread->running) thread->process();
		thread->running = 0;
		pthread_exit(NULL);
		return NULL;
	}
#endif

	int Thread::run(size_t size) {
		if (running) return 0;
		running = 1;
#ifdef _WIN32
		DWORD thread_id;
		thread = CreateThread(NULL, size, &Thread::thread_handler, this, 0, &thread_id);
		return (thread != NULL);
#else
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, size);
		pthread_create(&thread, &attr, &Thread::thread_handler, this);
		return (pthread_detach(thread) == 0);
#endif
	}

	int Thread::signal() {
		if (waiting == 0) return 0;
#ifdef _WIN32
		waiting = 0;
		return SetEvent(event);
#else
		pthread_mutex_lock(&mutex);
		waiting = 0;
		int ret = (pthread_cond_signal(&cond) == 0);
		pthread_mutex_unlock(&mutex);
		return ret;
#endif
	}

	int Thread::terminate() {
		if (running == 0) return 0;
		running = 0;
		waiting = 0;
#ifdef _WIN32
		return TerminateThread(thread, false);
#elif _LINUX
		pthread_mutex_lock(&mutex);
		int ret = (pthread_cancel(thread) == 0);
		pthread_mutex_unlock(&mutex);
		return ret;
#endif
	}

	/*
	 */
	void Thread::usleep(unsigned int usec) {
#ifdef _WIN32
		Sleep(usec / 1000);
#elif _LINUX
		::usleep(usec);
#endif
	}

	void Thread::wait() {
		if (running == 0) return;
		if (waiting) return;
#ifdef _WIN32
		waiting = 1;
		WaitForSingleObject(event, INFINITE);
#else
		pthread_mutex_lock(&mutex);
		waiting = 1;
		while (waiting) pthread_cond_wait(&cond, &mutex);
		pthread_mutex_unlock(&mutex);
#endif
	}

	/*
	 */
	int Thread::isRunning() const {
		return running;
	}

	int Thread::isWaiting() const {
		return waiting;
	}
}