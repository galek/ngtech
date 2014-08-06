#pragma once

#include "glew/glew.h"
#include "../Common/EString.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: GLExtensions
	//---------------------------------------------------------------------------
	class GLExtensions {
	public:
		static bool isExtSupported(const String &name);
		static void initExtensions();
	};
}