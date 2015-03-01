#include "CorePrivate.h"

#if PLATFORM_OS == PLATFORM_OS_WINDOWS
#define _WIN32_DCOM
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>
#include <intrin.h>
#pragma comment(lib,"wbemuuid.lib")
#ifdef HAVE_NVAPI
#include "../../../../Externals/RenderAdditions/NVApi/NVApi.h"
#pragma comment(lib,"nvapi.lib")
#endif

#ifdef HAVE_AMDADL
#include "../../../../Externals/RenderAdditions/ADL/adl_sdk.h"
#endif

#elif PLATFORM_OS == PLATFORM_OS_LINUX
#include <dlfcn.h>
#include <unistd.h>
#include <sys/sysctl.h>
#ifndef ARCH_ARM
#include <X11/Xlib.h>
#ifdef HAVE_NVCTRL
#include "../../../../Externals/RenderAdditions/NVCtrl/NVCtrl.h"
#include "../../../../Externals/RenderAdditions/NVCtrl/NVCtrlLib.h"
#endif
#ifdef HAVE_AMDADL
#define LINUX
#include "../../../../Externals/RenderAdditions/ADL/adl_sdk.h"
#endif
#endif
#elif PLATFORM_OS == PLATFORM_OS_MACOSX
#include <sys/sysctl.h>
#elif PLATFORM_OS == PLATFORM_OS_ANDROID
#include <dlfcn.h>
#elif PLATFORM_OS == PLATFORM_OS_IOS
#include <dlfcn.h>
#include <sys/sysctl.h>
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

#elif _MACOS

		strcpy(info,"MacOS");
#ifdef ARCH_X64
		strcat(info," 64bit");
#else
		strcat(info," 32bit");
#endif
#ifdef __clang__
		strcat(info," CLANG " GET_VERSION(__clang_major__) "." GET_VERSION(__clang_minor__) "." GET_VERSION(__clang_patchlevel__));
#elif __GNUC__
		strcat(info," GCC " GET_VERSION(__GNUC__) "." GET_VERSION(__GNUC_MINOR__) "." GET_VERSION(__GNUC_PATCHLEVEL__));
#endif

#elif _ANDROID

		strcpy(info,"Android");
#ifdef __clang__
		strcat(info," CLANG " GET_VERSION(__clang_major__) "." GET_VERSION(__clang_minor__) "." GET_VERSION(__clang_patchlevel__));
#elif __GNUC__
		strcat(info," GCC " GET_VERSION(__GNUC__) "." GET_VERSION(__GNUC_MINOR__) "." GET_VERSION(__GNUC_PATCHLEVEL__));
#endif

#elif _IOS

		strcpy(info,"iOS");
#ifdef __clang__
		strcat(info," CLANG " GET_VERSION(__clang_major__) "." GET_VERSION(__clang_minor__) "." GET_VERSION(__clang_patchlevel__));
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
#ifdef HAVE_AMDADL
	static void *__stdcall ADL_Main_Memory_Alloc(int size) {
		return malloc(size);
	}
#endif

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

#ifdef HAVE_NVAPI
		if (NvAPI_Initialize() == NVAPI_OK) {
			NvU32 num_handles = 0;
			NvPhysicalGpuHandle handles[NVAPI_MAX_PHYSICAL_GPUS];
			if (NvAPI_EnumPhysicalGPUs(handles, &num_handles) == NVAPI_OK && num_handles > 0) {
				for (NvU32 i = 0; i < num_handles; i++) {
					NV_DISPLAY_DRIVER_MEMORY_INFO_V2 memory_info;
					memory_info.version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER_2;
					if (NvAPI_GPU_GetMemoryInfo(handles[i], &memory_info) == NVAPI_OK) {
						int size = memory_info.dedicatedVideoMemory / 1024;
						if (memory_size < size) memory_size = size;
					}
				}
			}
		}
#endif

#ifdef HAVE_AMDADL
		HMODULE handle = LoadLibraryA("atiadlxx.dll");
		if(handle == NULL) handle = LoadLibraryA("atiadlxy.dll");
		if(handle != NULL) {
			typedef int *(*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK,int);
			typedef int *(*ADL_MAIN_CONTROL_DESTROY)();
			typedef int *(*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int*);
			typedef int *(*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo,int);
			typedef int *(*ADL_ADAPTER_ACTIVE_GET)(int,int*);
			typedef int *(*ADL_ADAPTER_MEMORYINFO_GET)(int,ADLMemoryInfo*);
			ADL_MAIN_CONTROL_CREATE ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)GetProcAddress(handle,"ADL_Main_Control_Create");
			ADL_MAIN_CONTROL_DESTROY ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)GetProcAddress(handle,"ADL_Main_Control_Destroy");
			ADL_ADAPTER_NUMBEROFADAPTERS_GET ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(handle,"ADL_Adapter_NumberOfAdapters_Get");
			ADL_ADAPTER_ADAPTERINFO_GET ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)GetProcAddress(handle,"ADL_Adapter_AdapterInfo_Get");
			ADL_ADAPTER_MEMORYINFO_GET ADL_Adapter_MemoryInfo_Get = (ADL_ADAPTER_MEMORYINFO_GET)GetProcAddress(handle,"ADL_Adapter_MemoryInfo_Get");
			if(ADL_Main_Control_Create != NULL && ADL_Main_Control_Destroy != NULL && ADL_Adapter_NumberOfAdapters_Get != NULL && ADL_Adapter_AdapterInfo_Get != NULL && ADL_Adapter_MemoryInfo_Get != NULL) {
				if(ADL_Main_Control_Create(ADL_Main_Memory_Alloc,1) == ADL_OK) {
					int num_adapters = 0;
					if(ADL_Adapter_NumberOfAdapters_Get(&num_adapters) == ADL_OK && num_adapters > 0) {
						int *devices = new int[num_adapters];
						AdapterInfo *adapter_info = new AdapterInfo[num_adapters];
						if(ADL_Adapter_AdapterInfo_Get(adapter_info,sizeof(AdapterInfo) * num_adapters) == ADL_OK) {
							for(int i = 0; i < num_adapters; i++) {
								ADLMemoryInfo memory_info;
								if(ADL_Adapter_MemoryInfo_Get(adapter_info[i].iAdapterIndex,&memory_info) == ADL_OK) {
									int size = (int)(memory_info.iMemorySize / 1024 / 1024);
									if(memory_size < size) memory_size = size;
								}
							}
						}
						delete [] adapter_info;
						delete [] devices;
					}
				}
			}
		}
#endif

#elif _LINUX

#ifdef HAVE_NVCTRL
		Display *display = XOpenDisplay(NULL);
		if (display) {
			int event_base, error_base;
			if (XNVCTRLQueryExtension(display, &event_base, &error_base)) {
				int num_screens = ScreenCount(display);
				for (int i = 0; i < num_screens; i++) {
					if (XNVCTRLIsNvScreen(display, i)) {
						char *str = NULL;
						int value = 0;
						if (XNVCTRLQueryStringAttribute(display, i, 0, NV_CTRL_STRING_PRODUCT_NAME, &str)) {
							if(strcmp(info,str)) {
								if(strlen(info)) strcat(info,"/");
								strcat(info,str);
							} else {
								count++;
							}
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
							int size = value / 1024;
							if(memory_size < size) memory_size = size;
						}
					}
				}
			}
			XCloseDisplay(display);
		}
#endif

#ifdef HAVE_AMDADL
		void *handle = dlopen("libatiadlxx.so",RTLD_LAZY);
		if(handle != NULL) {
			typedef int *(*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK,int);
			typedef int *(*ADL_MAIN_CONTROL_DESTROY)();
			typedef int *(*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int*);
			typedef int *(*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo,int);
			typedef int *(*ADL_ADAPTER_ACTIVE_GET)(int,int*);
			typedef int *(*ADL_ADAPTER_MEMORYINFO_GET)(int,ADLMemoryInfo*);
			ADL_MAIN_CONTROL_CREATE ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)dlsym(handle,"ADL_Main_Control_Create");
			ADL_MAIN_CONTROL_DESTROY ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)dlsym(handle,"ADL_Main_Control_Destroy");
			ADL_ADAPTER_NUMBEROFADAPTERS_GET ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)dlsym(handle,"ADL_Adapter_NumberOfAdapters_Get");
			ADL_ADAPTER_ADAPTERINFO_GET ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)dlsym(handle,"ADL_Adapter_AdapterInfo_Get");
			ADL_ADAPTER_ACTIVE_GET ADL_Adapter_Active_Get = (ADL_ADAPTER_ACTIVE_GET)dlsym(handle,"ADL_Adapter_Active_Get");
			ADL_ADAPTER_MEMORYINFO_GET ADL_Adapter_MemoryInfo_Get = (ADL_ADAPTER_MEMORYINFO_GET)dlsym(handle,"ADL_Adapter_MemoryInfo_Get");
			if(ADL_Main_Control_Create != NULL && ADL_Main_Control_Destroy != NULL && ADL_Adapter_NumberOfAdapters_Get != NULL && ADL_Adapter_AdapterInfo_Get != NULL && ADL_Adapter_Active_Get != NULL && ADL_Adapter_MemoryInfo_Get != NULL) {
				if(ADL_Main_Control_Create(ADL_Main_Memory_Alloc,1) == ADL_OK) {
					int num_adapters = 0;
					if(ADL_Adapter_NumberOfAdapters_Get(&num_adapters) == ADL_OK && num_adapters > 0) {
						int num_devices = 0;
						int *devices = new int[num_adapters];
						AdapterInfo *adapter_info = new AdapterInfo[num_adapters];
						if(ADL_Adapter_AdapterInfo_Get(adapter_info,sizeof(AdapterInfo) * num_adapters) == ADL_OK) {
							for(int i = 0; i < num_adapters; i++) {
								int active = 0;
								if(ADL_Adapter_Active_Get(adapter_info[i].iAdapterIndex,&active) != ADL_OK) continue;
								for(int j = 0; j < num_devices; j++) {
									if(devices[j] == adapter_info[i].iDeviceNumber) {
										active = ADL_FALSE;
										break;
									}
								}
								if(active != ADL_TRUE) continue;
								devices[num_devices++] = adapter_info[i].iDeviceNumber;
								char *name = adapter_info[i].strAdapterName;
								size_t length = strlen(adapter_info[i].strAdapterName);
								while(length > 0 && name[length - 1] == ' ') name[length - 1] = '\0';
								if(strcmp(info,name)) {
									if(strlen(info)) strcat(info,"/");
									strcat(info,name);
								} else {
									count++;
								}
								ADLMemoryInfo memory_info;
								if(ADL_Adapter_MemoryInfo_Get(adapter_info[i].iAdapterIndex,&memory_info) == ADL_OK) {
									int size = (int)(memory_info.iMemorySize / 1024 / 1024);
									if(memory_size < size) memory_size = size;
								}
							}
						}
						delete [] adapter_info;
						delete [] devices;
					}
				}
			}
		}
#endif

#elif _MACOS

		FILE *file = popen("system_profiler SPDisplaysDataType","r");
		if(file) {
			char *d = info;
			char buf[1024];
			while(fgets(buf,sizeof(buf),file)) {
				char *s = buf;
				while(*s == ' ') s++;
				if(!strncmp(s,"Chipset Model: ",15)) {
					s += 15;
					if(d != info) *d++ = '/';
					while(*s && *s != '\n') *d++ = *s++;
				} else if(!strncmp(s,"VRAM (Total): ",14)) {
					s += 14;
					int size = atoi(s);
					while(isdigit(*s)) s++;
					while(*s == ' ') s++;
					if(!strncmp(s,"GB",2)) size *= 1024;
					if(memory_size < size) memory_size = size;
				}
			}
			pclose(file);
		} else {
			Log::error("get_gpu_info(): can't run \"%s\"\n","system_profiler SPDisplaysDataType");
		}

#elif _ANDROID

		FILE *file = fopen("/proc/meminfo","rb");
		if(file) {
			char buf[1024];
			while(fgets(buf,sizeof(buf),file)) {
				if(!strncmp(buf,"MemTotal:",9)) {
					memory_size = (atoi(buf + 9) / 1024) / 4;
					if(memory_size > 256) memory_size = 256;
					break;
				}
			}
			fclose(file);
		} else {
			Log::error("get_gpu_info(): can't open \"%s\" file\n","/proc/meminfo");
		}

#elif _IOS

		int64_t data = 0;
		size_t size = sizeof(data);
		if(sysctlbyname("hw.memsize",&data,&size,NULL,0) == 0) {
			memory_size = (data / 1024 / 1024) / 4;
			if(memory_size > 256) memory_size = 256;
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

	int SystemInfo::hasAVX() {
		assert(initialized);
		return (strstr(cpu_info, "AVX") != NULL);
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