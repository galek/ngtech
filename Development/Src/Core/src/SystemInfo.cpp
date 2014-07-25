#include "CorePrivate.h"

#ifdef _WIN32
#define _WIN32_DCOM
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>
#include <intrin.h>
#pragma comment(lib,"wbemuuid.lib")
#ifdef HAVE_NVCPL
#include <NvCpl/NvCpl.h>
#endif
#ifdef HAVE_ATIMGPU
#include <atimgpud.h>
#pragma comment(lib,"atimgpud.lib")
#endif
#elif _LINUX
#include <dlfcn.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#ifdef HAVE_NVCTRL
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>
#endif
#endif

#include "SystemInfo.h"

namespace NGTech {
	/******************************************************************************\
	*
	* Return binary information
	*
	\******************************************************************************/

	/*
	 */
	static const char *get_binary_info() {

		static char info[1024];
		memset(info, 0, sizeof(info));

#define GET_NAME(NAME) #NAME
#define GET_VERSION(VERSION) GET_NAME(VERSION)

#ifdef _WIN32

		strcpy(info, "Windows");
#ifdef _WIN64
		strcat(info," 64bit");
#else
		strcat(info, " 32bit");
#endif
#ifdef __INTEL_COMPILER
		strcat(info," Intel C++ " GET_VERSION(__INTEL_COMPILER));
#elif _MSC_VER
		strcat(info, " Visual C++ " GET_VERSION(_MSC_VER));
#elif __GNUC__
		strcat(info," GCC " GET_VERSION(__GNUC__) "." GET_VERSION(__GNUC_MINOR__) "." GET_VERSION(__GNUC_PATCHLEVEL__));
#endif

#elif _LINUX

		strcpy(info,"Linux");
#ifdef _LP64
		strcat(info," 64bit");
#else
		strcat(info," 32bit");
#endif
#ifdef __INTEL_COMPILER
		strcat(info," Intel C++ " GET_VERSION(__INTEL_COMPILER));
#elif __GNUC__
		strcat(info," GCC " GET_VERSION(__GNUC__) "." GET_VERSION(__GNUC_MINOR__) "." GET_VERSION(__GNUC_PATCHLEVEL__));
#endif

#else

		strcpy(info,"Unknown");

#endif

#ifdef NDEBUG
		strcat(info," Release");
#else
		strcat(info, " Debug");
#endif
	
#undef GET_NAME
#undef GET_VERSION

		return info;
	}

	/******************************************************************************\
	*
	* Return system information
	*
	\******************************************************************************/

	/*
	 */
	static const char *get_system_info(int &memory_size) {

		static char info[1024];
		memset(info, 0, sizeof(info));

		memory_size = 256;

#ifdef _WIN32

		OSVERSIONINFO version;
		memset(&version, 0, sizeof(version));
		version.dwOSVersionInfoSize = sizeof(version);

		if (GetVersionEx(&version)) {
			switch (version.dwPlatformId) {
			case VER_PLATFORM_WIN32_NT:
				if (version.dwMajorVersion == 5) {
					switch (version.dwMinorVersion) {
					case 0: sprintf(info, "Windows 2000 (build %ld", version.dwBuildNumber); break;
					case 1: sprintf(info, "Windows XP (build %ld", version.dwBuildNumber); break;
					case 2: sprintf(info, "Windows Server 2003 (build %ld", version.dwBuildNumber); break;
					default: sprintf(info, "Windows NT %ld.%ld (build %ld", version.dwMajorVersion, version.dwMinorVersion, version.dwBuildNumber);
					}
				}
				else if (version.dwMajorVersion == 6) {
					switch (version.dwMinorVersion) {
					case 0: sprintf(info, "Windows Vista (build %ld", version.dwBuildNumber); break;
					case 1: sprintf(info, "Windows 7 (build %ld", version.dwBuildNumber); break;
					default: sprintf(info, "Windows NT %ld.%ld (build %ld", version.dwMajorVersion, version.dwMinorVersion, version.dwBuildNumber);
					}
				}
				else {
					sprintf(info, "Windows NT %ld.%ld (build %ld", version.dwMajorVersion, version.dwMinorVersion, version.dwBuildNumber);
				}
				if (version.szCSDVersion[0] != '\0') {
					strcat(info, ", ");
					strcat(info, version.szCSDVersion);
				}
				strcat(info, ")");
				break;
			case VER_PLATFORM_WIN32_WINDOWS:
				switch (version.dwMinorVersion) {
				case 0:	strcpy(info, "Windows 95"); break;
				case 10: strcpy(info, "Windows 98"); break;
				case 90: strcpy(info, "Windows ME"); break;
				default: sprintf(info, "Windows 9x %ld.%ld", version.dwMajorVersion, version.dwMinorVersion);
				}
				break;
			default:
				strcpy(info, "Unknown Windows OS");
			}
		}

#ifdef _WIN64
		strcat(info," 64bit");
#else
		BOOL is_wow64_process = FALSE;
		typedef BOOL(WINAPI *ISWOW64PROCESS)(HANDLE, PBOOL);
		ISWOW64PROCESS IsWow64Process = (ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"), "IsWow64Process");
		if (IsWow64Process) IsWow64Process(GetCurrentProcess(), &is_wow64_process);
		if (is_wow64_process) strcat(info, " 64bit");
		else strcat(info, " 32bit");
#endif

		MEMORYSTATUS memory;
		memset(&memory, 0, sizeof(memory));
		memory.dwLength = sizeof(memory);
		GlobalMemoryStatus(&memory);
		memory_size = (int)(memory.dwTotalPhys / 1024 / 1024);

#elif _LINUX

		FILE *file = popen("uname -s -r -m","r");
		if(file) {
			size_t size = fread(info,1,sizeof(info),file);
			while(size > 0 && info[size - 1] == '\n') size--;
			info[size] = '\0';
			pclose(file);
		}

		file = fopen("/proc/meminfo","r");
		if(file) {
			char buf[1024];
			while(fgets(buf,sizeof(buf),file)) {
				if(!strncmp(buf,"MemTotal:",9)) {
					sscanf(buf + 9,"%d",&memory_size);
					memory_size /= 1024;
					break;
				}
			}
			fclose(file);
		} else 
			Error("get_system_info(): can't open \"%s\" file\n","/proc/meminfo",true);
#endif

		if (info[0] == '\0') {
			strcpy(info, "Unknown System");
		}

		return info;
	}

	/******************************************************************************\
	*
	* Return CPU information
	*
	\******************************************************************************/

	/*
	 */
#if defined(_WIN32) || defined(_LINUX)
	static ENGINE_INLINE void int2str(char *dest, unsigned int value) {
		dest[0] = (value >> 0) & 0xff;
		dest[1] = (value >> 8) & 0xff;
		dest[2] = (value >> 16) & 0xff;
		dest[3] = (value >> 24) & 0xff;
	}
#ifdef _WIN32
	static ENGINE_INLINE void cpuid(int *dest, int func) {
		__cpuid(dest, func);
	}
#elif _LINUX
#ifdef _LP64
	static ENGINE_INLINE void cpuid(int *dest,int func) {
		asm volatile("cpuid" : "=a"(dest[0]), "=b"(dest[1]), "=c"(dest[2]), "=d"(dest[3]) : "a"(func));
	}
#else
	static ENGINE_INLINE void cpuid(int *dest,int func) {
		asm volatile("pushl %%ebx\ncpuid\nmovl %%ebx, %%esi\npopl %%ebx" : "=a"(dest[0]), "=S"(dest[1]), "=c"(dest[2]), "=d"(dest[3]) : "a"(func));
	}
#endif
#endif
#endif

	/*
	 */
	static const char *get_cpu_info(int &frequency, int &count) {

		static char info[1024];
		memset(info, 0, sizeof(info));

		frequency = 1000;
		count = 1;

#if defined(_WIN32) || defined(_LINUX)

		struct CPUReg {
			unsigned int eax;
			unsigned int ebx;
			unsigned int ecx;
			unsigned int edx;
		};

		CPUReg reg;

		cpuid((int*)&reg, 0x00);
		unsigned int eax = reg.eax;
		int2str(info + 0, reg.ebx);
		int2str(info + 4, reg.edx);
		int2str(info + 8, reg.ecx);
		info[12] = '\0';

		if (!strcmp(info, "AuthenticAMD")) strcpy(info, "AMD CPU");
		else if (!strcmp(info, "GenuineIntel")) strcpy(info, "Intel CPU");

		cpuid((int*)&reg, 0x80000000);
		unsigned int ext_eax = reg.eax;
		unsigned int ext_ecx = 0;
		unsigned int ext_edx = 0;
		if (ext_eax >= 0x80000001) {
			cpuid((int*)&reg, 0x80000001);
			ext_ecx = reg.ecx;
			ext_edx = reg.edx;
			for (int j = 0; j < 3 && 0x80000002 + j <= ext_eax; j++) {
				cpuid((int*)&reg, 0x80000002 + j);
				int2str(info + 16 * j + 0, reg.eax);
				int2str(info + 16 * j + 4, reg.ebx);
				int2str(info + 16 * j + 8, reg.ecx);
				int2str(info + 16 * j + 12, reg.edx);
				info[16 * j + 16] = '\0';
			}
		}

		char *s = info;
		while (*s == ' ') s++;
		strcpy(info, s);


#ifdef _WIN32

		SYSTEM_INFO system;
		GetSystemInfo(&system);
		count = system.dwNumberOfProcessors;

#elif _LINUX

		FILE *file = fopen("/proc/cpuinfo","r");
		if(file) {
			count = 0;
			char buf[1024];
			while(fgets(buf,sizeof(buf),file)) {
				char *s = strchr(buf,':');
				if(s++ == NULL) continue;
				while(*s == ' ') s++;
				if(!strncmp(buf,"processor",9)) {
					sscanf(s,"%d",&count);
					count++;
				}
			}
			fclose(file);
		} else {
			Error("get_cpu_info(): can't open \"%s\" file\n","/proc/cpuinfo",true);
		}

#endif	
#endif

		if (info[0] == '\0') {
			strcpy(info, "Unknown CPU");
		}

		return info;
	}

	/******************************************************************************\
	*
	* Return GPU information
	*
	\******************************************************************************/

	/*
	 */
	static const char *get_gpu_info(int &memory_size, int &count) {

		static char info[1024];
		memset(info, 0, sizeof(info));

		memory_size = 256;
		count = 1;

#ifdef _WIN32
#ifdef HAVE_NVCPL
		HINSTANCE nvcpl = LoadLibrary("NVCPL.dll");
		if (nvcpl != NULL) {
			NvCplGetDataIntType NvCplGetDataInt = (NvCplGetDataIntType)GetProcAddress(nvcpl, "NvCplGetDataInt");
			if (NvCplGetDataInt != NULL) {
				long value = 0;
				if (NvCplGetDataInt(NV_DATA_TYPE_VIDEO_MEMORY_SIZE, &value)) {
					memory_size = (int)value;
				}
				if (NvCplGetDataInt(NVCPL_API_NUMBER_OF_GPUS, &value)) {
					count = (int)value;
				}
				if (NvCplGetDataInt(NVCPL_API_SLI_MULTI_GPU_RENDERING_MODE, &value)) {
					if (value & NVCPL_API_SLI_ENABLED) {
						if (value & NVCPL_API_SLI_RENDERING_MODE_AUTOSELECT) strcat(info, " SLI Auto");
						else if (value & NVCPL_API_SLI_RENDERING_MODE_AFR) strcat(info, " SLI AFR");
						else if (value & NVCPL_API_SLI_RENDERING_MODE_SFR) strcat(info, " SLI SFR");
						else if (value & NVCPL_API_SLI_RENDERING_MODE_SINGLE_GPU) strcat(info, " SLI Single");
					}
				}
			}
			FreeLibrary(nvcpl);
		}
#endif

#ifdef HAVE_ATIMGPU
		count = AtiMultiGPUAdapters();
		if (count > 3) strcat(info, " QuadFireX");
		else if (count > 1) strcat(info, " CrossFireX");
#endif

#elif _LINUX

		Display *display = XOpenDisplay(NULL);

#ifdef HAVE_NVCTRL
		if (display) {
			int event_base, error_base;
			if (XNVCTRLQueryExtension(display, &event_base, &error_base)) {
				int num_screens = ScreenCount(display);
				for (int i = 0; i < num_screens; i++) {
					if (XNVCTRLIsNvScreen(display, i)) {
						char *str = NULL;
						int value = 0;
						if (XNVCTRLQueryStringAttribute(display, i, 0, NV_CTRL_STRING_PRODUCT_NAME, &str)) {
							strcpy(info, str);
							XFree(str);
						}
						if(XNVCTRLQueryAttribute(display,i,0,NV_CTRL_BUS_TYPE,&value)) {
							if(value == NV_CTRL_BUS_TYPE_AGP) strcat(info," AGP");
							else if(value == NV_CTRL_BUS_TYPE_PCI) strcat(info," PCI");
							else if(value == NV_CTRL_BUS_TYPE_PCI_EXPRESS) strcat(info," PCI Express");
						}
						if(XNVCTRLQueryStringAttribute(display,i,0,NV_CTRL_STRING_NVIDIA_DRIVER_VERSION,&str)) {
							strcat(info," ");
							strcat(info,str);
							XFree(str);
						}
						if(XNVCTRLQueryAttribute(display,i,0,NV_CTRL_VIDEO_RAM,&value)) {
							memory_size = value / 1024;
						}
					}
				}
			}
		}
#endif

		if(display) {
			if(info[0] == '\0') {
				void *handle = dlopen("libGL.so.1",RTLD_GLOBAL | RTLD_LAZY);
				if (handle == NULL) handle = dlopen("libGL.so.1", RTLD_LAZY);
				if(handle != NULL) {
					typedef XVisualInfo *(*GLXCHOOSEVISUAL)(Display*,int,int*);
					typedef GLXContext (*GLXCREATECONTEXT)(Display*,XVisualInfo*,GLXContext,Bool);
					typedef void (*GLXDESTROYCONTEXT)(Display*,GLXContext);
					typedef Bool (*GLXMAKECURRENT)(Display*,GLXDrawable,GLXContext);
					typedef const char *(APIENTRY *GLGETSTRING)(GLenum);
					typedef void (APIENTRY *GLGETINTEGERV)(GLenum,GLint*);
					GLXCHOOSEVISUAL dlglXChooseVisual = (GLXCHOOSEVISUAL)dlsym(handle,"glXChooseVisual");
					GLXCREATECONTEXT dlglXCreateContext = (GLXCREATECONTEXT)dlsym(handle,"glXCreateContext");
					GLXDESTROYCONTEXT dlglXDestroyContext = (GLXDESTROYCONTEXT)dlsym(handle,"glXDestroyContext");
					GLXMAKECURRENT dlglXMakeCurrent = (GLXMAKECURRENT)dlsym(handle,"glXMakeCurrent");
					GLGETSTRING dlglGetString = (GLGETSTRING)dlsym(handle,"glGetString");
					GLGETINTEGERV dlglGetIntegerv = (GLGETINTEGERV)dlsym(handle,"glGetIntegerv");
					int screen = DefaultScreen(display);
					int attribs[] = {
						GLX_RGBA, GLX_DOUBLEBUFFER,
						GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8,
						GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8,
						0,
					};
					XVisualInfo *visual = dlglXChooseVisual(display,screen,attribs);
					GLXContext context = dlglXCreateContext(display,visual,NULL,GL_TRUE);
					if(context) {
						XSetWindowAttributes attr;
						memset(&attr,0,sizeof(attr));
						Window root_window = RootWindow(display,screen);
						attr.colormap = XCreateColormap(display,root_window,visual->visual,AllocNone);
						Window window = XCreateWindow(display,root_window,0,0,1,1,0,visual->depth,InputOutput,visual->visual,CWColormap,&attr);
						if(dlglXMakeCurrent(display,window,context)) {
							strcpy(info,dlglGetString(GL_RENDERER));
							strcat(info," ");
							strcat(info,dlglGetString(GL_VERSION));
							if(strstr(dlglGetString(GL_EXTENSIONS),"ATI_meminfo")) {
								GLint params[4];
								dlglGetIntegerv(0x87fc,params);
								params[0] /= 1024;
								memory_size = 1;
								while(memory_size < params[0]) {
									memory_size *= 2;
								}
							}
						} else {
							Error("get_gpu_info(): can't set OpenGL context\n",true);
						}
						dlglXDestroyContext(display,context);
						XDestroyWindow(display,window);
					} else {
						Error("get_gpu_info(): can't create OpenGL context\n",true);
					}
					XFree(visual);
				} else {
					Error("get_gpu_info(): can't load \"%s\" library\n%s\n","libGL.so.1",true,dlerror());
				}
			}
		}

		if(display) {
			XCloseDisplay(display);
		}

#endif

		if (info[0] == '\0') {
			strcpy(info, "Unknown GPU");
		}

		return info;
	}

	/*
	 */
	static int initialized = 0;

	static const char *binary_info = NULL;

	static const char *system_info = NULL;
	static int system_memory = 0;

	static const char *cpu_info = NULL;
	static int cpu_frequency = 0;
	static int cpu_count = 0;

	static const char *gpu_info = NULL;
	static int gpu_memory = 0;
	static int gpu_count = 0;

	/*
	*/
	SystemInfo::SystemInfo(){
		init();
	}

	/*
	 */
	int SystemInfo::init() {

		assert(initialized == 0 && "SystemInfo::init(): is already initialized");
		initialized = 1;

		binary_info = get_binary_info();
		system_info = get_system_info(system_memory);
		cpu_info = get_cpu_info(cpu_frequency, cpu_count);
		gpu_info = get_gpu_info(gpu_memory, gpu_count);

		return (system_info && cpu_info && gpu_info);
	}

	/*
	*/
	void SystemInfo::shutdown() {

		assert(initialized == 1 && "SystemInfo::shutdown(): is not initialized");
		initialized = 0;

		system_info = NULL;
		system_memory = 0;

		cpu_info = NULL;
		cpu_frequency = 0;

		gpu_info = NULL;
		gpu_memory = 0;
		gpu_count = 0;
	}

	/*
	 */
	const char *SystemInfo::getBinaryInfo() {
		assert(initialized);
		return binary_info;
	}

	/*
	 */
	const char *SystemInfo::getSystemInfo() {
		assert(initialized);
		return system_info;
	}

	int SystemInfo::getSystemMemory() {
		assert(initialized);
		return system_memory;
	}

	/*
	 */
	const char *SystemInfo::getCPUInfo() {
		assert(initialized);
		return cpu_info;
	}

	int SystemInfo::getCPUFrequency() {
		assert(initialized);
		return cpu_frequency;
	}

	int SystemInfo::getCPUCount() {
		assert(initialized);
		return cpu_count;
	}

	int SystemInfo::hasMMX() {
		assert(initialized);
		return (strstr(cpu_info, "MMX") != NULL);
	}

	int SystemInfo::hasSSE() {
		assert(initialized);
		return (strstr(cpu_info, "SSE") != NULL);
	}

	int SystemInfo::hasSSE2() {
		assert(initialized);
		return (strstr(cpu_info, "SSE2") != NULL);
	}

	int SystemInfo::hasSSE3() {
		assert(initialized);
		return (strstr(cpu_info, "SSE3") != NULL);
	}

	int SystemInfo::hasSSE4() {
		assert(initialized);
		return (strstr(cpu_info, "SSE4") != NULL);
	}

	int SystemInfo::hasSSE5() {
		assert(initialized);
		return (strstr(cpu_info, "SSE5") != NULL);
	}

	int SystemInfo::has3DNow() {
		assert(initialized);
		return (strstr(cpu_info, "3DNow!") != NULL);
	}

	int SystemInfo::hasAltiVec() {
		return (strstr(cpu_info, "AltiVec") != NULL);
	}

	/*
	 */
	const char *SystemInfo::getGPUInfo() {
		assert(initialized);
		return gpu_info;
	}

	int SystemInfo::getGPUMemory() {
		assert(initialized);
		return gpu_memory;
	}

	int SystemInfo::getGPUCount() {
		assert(initialized);
		return gpu_count;
	}
}