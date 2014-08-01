/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/
#pragma once
 

#ifdef WIN32
#include <windows.h>
#endif

//***************************************************************************
#include "../Includes/AL/al.h"
#include "../Includes/AL/alc.h"
//***************************************************************************
#include "../Includes/GL/gl.h"
#include "../Includes/GL/glext.h"
//***************************************************************************
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;