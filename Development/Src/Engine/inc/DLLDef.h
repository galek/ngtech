#ifndef DLL_DEF
#define DLL_DEF

#ifdef VEGA_EXPORTS
#define VEGA_API __declspec(dllexport)
#else
#define VEGA_API __declspec(dllimport)
#endif

#endif