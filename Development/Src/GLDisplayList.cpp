/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLDisplayList.h"
//***************************************************************************

namespace VEGA {

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

