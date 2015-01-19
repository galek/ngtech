#pragma once

#include "glew/glew.h"
#include "../Common/StringHelper.h"
//***************************************************************************

namespace NGTech {

	/**
	OpenGL extensions managing class
	*/
	class GLExtensions {
	public:
		/**
		Is GL extension supported
		\param name extension name
		\return true if supported
		*/
		static bool isExtSupported(const char*name);
		/**
		Init extensions
		*/
		static void initExtensions();
	};
}