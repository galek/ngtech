#pragma once

#include "../../../../Externals/RenderAdditions/glew/GL/glew.h"
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
#include "../../../../Externals/RenderAdditions/glew/gl/wglew.h"
#else
#include "../../../../Externals/RenderAdditions/glew/GL/glxew.h"
#endif
//***************************************************************************

namespace NGTech {

	/**
	OpenGL extensions managing class
	*/
	struct GLExtensions 
	{
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