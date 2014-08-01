#include "CorePrivate.h"
//***************************************************************************
#include "../../../Externals/ResIL/include/IL/il.h"
#include "../../../Externals/ResIL/include/IL/ilu.h"
//***************************************************************************

namespace NGTech {
	/**
	*/
	CORE_API void ILSystemInit()
	{
		ilInit();
		iluInit();
	}

	/**
	*/
}