#include "RenderPrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLDisplayList.h"
//***************************************************************************

namespace NGTech {

	/*
	*/
	GLDisplayList::GLDisplayList() {
		glID = glGenLists(1);
	}

	/*
	*/
	GLDisplayList::~GLDisplayList() {
		glDeleteLists(glID, 1);
	}

	/*
	*/
	void GLDisplayList::beginBuild() {
		glNewList(glID, GL_COMPILE);
	}

	/*
	*/
	void GLDisplayList::endBuild() {
		glEndList();
	}

	/*
	*/
	void GLDisplayList::call() {
		glCallList(glID);
	}

}
