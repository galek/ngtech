#include "RenderPrivate.h"
//***************************************************************************
#include "GLExtensions.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	bool GLExtensions::isExtSupported(const String &name) {
		String ext = (char *)glGetString(GL_EXTENSIONS);

		for (int i = 0; i < StringHelper::getWordsNumber(ext); i++) {
			if (StringHelper::getWord(ext, i + 1) == name) return true;
		}

		return false;
	}

	/**
	*/
	void GLExtensions::initExtensions() {
		if (glewInit() != GLEW_OK)
			Error("GLExtensions::initExtensions()", true);
	}

}
