#include "CorePrivate.h"
//***************************************************************************
#include "ILImage.h"
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