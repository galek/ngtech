#pragma once
 
//***************************************************************************
#include "../../../Externals/openal/include/AL/al.h"
#include "../../../Externals/openal/include/AL/alc.h"
//***************************************************************************
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;