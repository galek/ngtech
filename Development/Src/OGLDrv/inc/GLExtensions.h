#pragma once

#include "glew/glew.h"
#include "../Common/StringHelper.h"
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