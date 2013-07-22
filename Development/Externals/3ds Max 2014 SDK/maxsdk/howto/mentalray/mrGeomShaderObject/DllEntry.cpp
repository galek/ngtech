/*==============================================================================

  file:     DllEntry.cpp

  author:   Daniel Levesque

  created:  16 June 2003

  description:

    Dll entry point.

  modified:	


� 2004 Autodesk
==============================================================================*/

#include "mrGeomShaderObject.h"
#include "Utility.h"
#include "resource.h"
#include <iparamb2.h>

HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
   if( fdwReason == DLL_PROCESS_ATTACH )
   {
      MaxSDK::Util::UseLanguagePackLocale();
      hInstance = hinstDLL;				// Hang on to this DLL's instance handle.
      DisableThreadLibraryCalls(hInstance);
   }

	return (TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESCRIPTION);
}

__declspec( dllexport ) int LibNumberClasses()
{
	return 1;;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) {
		case 0: 
            return &mrGeomShaderObject::GetClassDesc();
		default: 
            return NULL;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}