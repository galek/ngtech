//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 05/20/2001 <--Y2K Compliant! =]
//
// Filename: src-ILU/src/ilu_main.c
//
// Description: Startup functions
//
//-----------------------------------------------------------------------------


#include "../include/ilu_internal.h"
#include "../include/ilu_states.h"
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif//_WIN32

#ifdef _WIN32
	#if (defined(IL_USE_PRAGMA_LIBS))
		#if defined(_MSC_VER) || defined(__BORLANDC__)
			#pragma comment(lib, "DevIL.lib")
		#endif
	#endif
#endif

void ILAPIENTRY iluInit()
{
	// Used mostly for rotations
	//IL_PI = 4 * atan(1);		// precomputed value of pi
	//IL_DEGCONV = IL_PI / 180;	// division is slow on some computers

	iluSetLanguage(ILU_ENGLISH);
	return;
}


//#ifndef _WIN32_WCE
ILuint ILAPIENTRY iluLoadImage(ILconst_string FileName)
{
	ILuint Id;
	ilGenImages(1, &Id);
	if (Id == 0)
		return 0;
	if (!ilLoadImage(FileName)) {
		ilDeleteImages(1, &Id);
		return 0;
	}
	return Id;
}
//#endif//_WIN32_WCE
