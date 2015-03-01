#pragma once

namespace NGTech {
	/*
	 */
	class CORE_API SystemInfo {
	public:
		SystemInfo();
		// initialize system info
		static int init();
		static void shutdown();

		// binary information string
		static const char *getBinaryInfo();

		// operating system information string
		static const char *getSystemInfo();

		// system memory in Mbytes
		static int getSystemMemory();

		// CPU information string
		static const char *getCPUInfo();

		// CPU frequency in MHz
		static int getCPUFrequency();

		// CPU counter
		static int getCPUCount();

		// CPU extensions
		static int hasMMX();
		static int hasSSE();
		static int hasSSE2();
		static int hasSSE3();
		static int hasSSE4();
		static int hasSSE5();
		static int hasAVX();
		static int has3DNow();
		static int hasAltiVec();

		// GPU information string
		static const char *getGPUInfo();

		// GPU memory in Mbytes
		static int getGPUMemory();

		// GPU counter
		static int getGPUCount();
	};
}