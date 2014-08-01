//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/07/2002 <--Y2K Compliant! =]
//
// Filename: src-ILUT/include/ilut_internal.h
//
// Description: Internal stuff for ILUT
//
//-----------------------------------------------------------------------------


#ifndef INTERNAL_H
#define INTERNAL_H

#define IL_STATIC_LIB

#ifdef HAVE_CONFIG_H //if we use autotools, we have HAVE_CONFIG_H defined and we have to look for it like that
#include <config.h>
#else // if we don't use autotools, we have to point to (possibly different) config.h than in the opposite case
#include "../../include/IL/config.h"
#endif

#include <string.h>

#ifdef _WIN32
#ifdef _MSC_VER
		#if _MSC_VER > 1000
			#pragma warning(disable: 4996)	// "The POSIX name for this item is deprecated." AND "This function or variable may be unsafe."
		#endif // _MSC_VER > 1000
	#endif
#endif

#include "../../include/IL/ilut.h"
#include "../../include/IL/devil_internal_exports.h"

#include <stdlib.h>

extern ILimage *ilutCurImage;

void	ilutDefaultStates(void);


#ifdef _UNICODE
	#define IL_TEXT(s) L##s
#else
	#define IL_TEXT(s) (s)
#endif


// ImageLib Utility Toolkit's OpenGL Functions
#ifdef ILUT_USE_OPENGL
	ILboolean ilutGLInit();
#endif

// ImageLib Utility Toolkit's Win32 Functions
#ifdef ILUT_USE_WIN32
	ILboolean ilutWin32Init();
#endif

// ImageLib Utility Toolkit's Win32 Functions
#ifdef ILUT_USE_DIRECTX8
	ILboolean ilutD3D8Init();
#endif

#ifdef ILUT_USE_DIRECTX9
	ILboolean ilutD3D9Init();
#endif

#ifdef ILUT_USE_DIRECTX10
	ILboolean ilutD3D10Init();
#endif

#define CUBEMAP_SIDES 6
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if((p)!=NULL){(p)->lpVtbl->Release(p);(p)=NULL;}}
#endif


#endif//INTERNAL_H