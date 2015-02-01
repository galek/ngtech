#include "RenderPrivate.h"
//***************************************************************************
#include "GLExtensions.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	bool GLExtensions::isExtSupported(const char*name) {
		return glewIsExtensionSupported(name);
	}

	/**
	*/
	void GLExtensions::initExtensions() {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			Error("GLExtensions::initExtensions()", true);
	}

}