#pragma once

#ifdef _WIN32
	#include <windows.h>
	#include <intrin.h>
#elif _LINUX
	#include <pthread.h>
#endif



namespace NGTech
{
	/*
	 */
	class CORE_API Thread {

	public:

		Thread();
		virtual ~Thread();

		int run(size_t size = 0x100000);
		int signal();
		int terminate();

		int isRunning() const;
		int isWaiting() const;

	protected:

		virtual void process() = 0;

		void usleep(unsigned int usec);
		void wait();

#ifdef _WIN32
		static DWORD WINAPI thread_handler(LPVOID data);
		HANDLE thread;
		HANDLE event;
#else
		static void *thread_handler(void *data);
		pthread_t thread;
		pthread_cond_t cond;
		pthread_mutex_t mutex;
#endif

		volatile int running;
		volatile int waiting;
	};

	/******************************************************************************\
	*
	* Atomic Compare and swap
	*
	\******************************************************************************/

	/*
	 */
	ENGINE_INLINE int AtomicCAS(volatile int *ptr, int old_value, int new_value) {
#ifdef _WIN32
		return (_InterlockedCompareExchange((long volatile*)ptr, new_value, old_value) == old_value);
#elif _LINUX
		return (__sync_val_compare_and_swap(ptr,old_value,new_value) == old_value);
#else
		if(*ptr != old_value) return 0;
		*ptr = new_value;
		return 1;
#endif
	}

	/*
	 */
	ENGINE_INLINE void SpinLock(volatile int *ptr, int old_value, int new_value) {
		while (!AtomicCAS(ptr, old_value, new_value));
	}

	/*
	 */
	ENGINE_INLINE void WaitLock(volatile int *ptr, int value) {
		while (!AtomicCAS(ptr, value, value));
	}

	/*
	 */
	class AtomicLock {

	public:

		ENGINE_INLINE AtomicLock(volatile int *ptr) : ptr(ptr) {
			SpinLock(ptr, 0, 1);
		}
		ENGINE_INLINE ~AtomicLock() {
			SpinLock(ptr, 1, 0);
		}

	private:

		volatile int *ptr;
	};
}