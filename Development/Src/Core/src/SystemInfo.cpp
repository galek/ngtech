#include "CorePrivate.h"

#if PLATFORM_OS == PLATFORM_OS_WINDOWS
#define _WIN32_DCOM
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>
#include <intrin.h>
#pragma comment(lib,"wbemuuid.lib")
#ifdef HAVE_NVCPL
#include "NvCpl/NvCpl.h"
#endif
#ifdef HAVE_ATIMGPU
#include "atimgpud/atimgpud.h"
#if _WIN64
#pragma comment(lib,"atimgpud/atimgpud_s_x64.lib")
#else
#pragma comment(lib,"atimgpud/atimgpud_s_x86.lib")
#endif
#endif
#elif PLATFORM_OS == PLATFORM_OS_LINUX
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

#if PLATFORM_OS == PLATFORM_OS_WINDOWS
		strcpy(info, "Windows");
#ifdef _WIN64
		strcat(info, " 64bit");
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

#elif PLATFORM_OS == PLATFORM_OS_LINUX

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

#ifdef _DEBUG
		strcat(info, " Debug");
#else
		strcat(info, " Release");
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
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
	int CompareWindowsVersion(DWORD dwMajorVersion, DWORD dwMinorVersion)
	{
		OSVERSIONINFOEX ver;
		DWORDLONG dwlConditionMask = 0;

		ZeroMemory(&ver, sizeof(OSVERSIONINFOEX));
		ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		ver.dwMajorVersion = dwMajorVersion;
		ver.dwMinorVersion = dwMinorVersion;

		VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
		VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

		return VerifyVersionInfo(&ver, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
	}
#endif
	/*
	 */
	static const char *get_system_info(int &memory_size) {

		static char info[1024];
		memset(info, 0, sizeof(info));

		memory_size = 256;

#if PLATFORM_OS == PLATFORM_OS_WINDOWS
		if (CompareWindowsVersion(6, 0))
			sprintf(info, "Windows Vista");
		else if (CompareWindowsVersion(6, 1))
			sprintf(info, "Windows 7");
		else if (CompareWindowsVersion(6, 2))
			sprintf(info, "Windows 8");
		else if (CompareWindowsVersion(6, 3))
			sprintf(info, "Windows 8.1");
		else if ((CompareWindowsVersion(6, 4)) || (CompareWindowsVersion(10, 0)))
			sprintf(info, "Windows 10");
#ifdef _WIN64
		strcat(info, " 64bit");
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

#elif PLATFORM_OS == PLATFORM_OS_LINUX

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
#if PLATFORM_OS == PLATFORM_OS_WINDOWS || PLATFORM_OS == PLATFORM_OS_LINUX
	static ENGINE_INLINE void int2str(char *dest, unsigned int value) {
		dest[0] = (value >> 0) & 0xff;
		dest[1] = (value >> 8) & 0xff;
		dest[2] = (value >> 16) & 0xff;
		dest[3] = (value >> 24) & 0xff;
	}
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
	static ENGINE_INLINE void cpuid(int *dest, int func) {
		__cpuid(dest, func);
	}
#elif PLATFORM_OS == PLATFORM_OS_LINUX
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

#if PLATFORM_OS == PLATFORM_OS_WINDOWS || PLATFORM_OS == PLATFORM_OS_LINUX

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


#if PLATFORM_OS == PLATFORM_OS_WINDOWS

		SYSTEM_INFO system;
		GetSystemInfo(&system);
		count = system.dwNumberOfProcessors;
		std::string str("Supported Threads:");
		strcpy(info, (str + std::to_string(count)).c_str());

#elif PLATFORM_OS == PLATFORM_OS_LINUX

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

#if PLATFORM_OS == PLATFORM_OS_WINDOWS

		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		IWbemLocator *locator = NULL;
		if (SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&locator))) {
			IWbemServices *services = NULL;
			if (SUCCEEDED(locator->ConnectServer(L"root\\cimv2", NULL, NULL, 0, NULL, 0, 0, &services))) {
				if (SUCCEEDED(CoSetProxyBlanket(services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE))) {
					IEnumWbemClassObject *enumerator = NULL;
					if (SUCCEEDED(services->ExecQuery(L"wql", L"select * from Win32_VideoController where DeviceID=\"VideoController1\"", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator))) {
						ULONG ret = 0;
						IWbemClassObject *object = NULL;
						while (enumerator->Next(WBEM_INFINITE, 1, &object, &ret) == WBEM_S_NO_ERROR && ret == 1) {
							VARIANT property;
							VariantInit(&property);
							if (SUCCEEDED(object->Get(L"Caption", 0, &property, 0, 0))) {
								char *d = info;
								const wchar_t *s = property.bstrVal;
								while (s && *s) *d++ = (*s++) & 0xff;
								*d = '\0';
							}
							VariantClear(&property);
							VariantInit(&property);
							if (SUCCEEDED(object->Get(L"DriverVersion", 0, &property, 0, 0))) {
								const wchar_t *s = property.bstrVal;
								char *d = info + strlen(info);
								if (d != info) *d++ = ' ';
								while (s && *s) *d++ = (*s++) & 0xff;
								*d = '\0';
							}
							VariantClear(&property);
							VariantInit(&property);
							if (SUCCEEDED(object->Get(L"AdapterRAM", 0, &property, 0, 0))) {
								memory_size = property.uintVal / 1024 / 1024;
							}
							VariantClear(&property);
							object->Release();
						}
						enumerator->Release();
					}
				}
				services->Release();
			}
			locator->Release();
		}
		if (hr == S_OK) CoUninitialize();

#if HAVE_NVCPL
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

#if HAVE_ATIMGPU
		count = AtiMultiGPUAdapters();
		if(count > 3) strcat(info," QuadFireX");
		else if(count > 1) strcat(info," CrossFireX");
#endif
#elif PLATFORM_OS == PLATFORM_OS_LINUX

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
			strcpy(info, "Additional GPU information is not available");
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