#pragma once
 
//***************************************************************************
#include "../../Includes/AL/al.h"
#include "../../Includes/AL/alc.h"
//***************************************************************************
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;