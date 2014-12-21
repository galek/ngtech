#include "RenderPrivate.h"
//***************************************************************************
#include "GLExtensions.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	bool GLExtensions::isExtSupported(const String &name) {
		return glewIsExtensionSupported(name.c_str());
	}

	/**
	*/
	void GLExtensions::initExtensions() {
		if (glewInit() != GLEW_OK)
			Error("GLExtensions::initExtensions()", true);
	}

}
