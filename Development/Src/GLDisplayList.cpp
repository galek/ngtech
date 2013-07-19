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

	//---------------------------------------------------------------------------
	//Desc:    creates GLDisplayList
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLDisplayList *GLDisplayList::create() {
		GLDisplayList *list = new GLDisplayList();

		list->glID = glGenLists(1);
		return list;
	}

	//---------------------------------------------------------------------------
	//Desc:    GLDisplayList destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLDisplayList::~GLDisplayList() {
		glDeleteLists(glID, 1);
	}

	//---------------------------------------------------------------------------
	//Desc:    begins the display list build
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLDisplayList::beginBuild() {
		glNewList(glID, GL_COMPILE);		
	}

	//---------------------------------------------------------------------------
	//Desc:    ends the display list build
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLDisplayList::endBuild() {
		glEndList();
	}

	//---------------------------------------------------------------------------
	//Desc:    calls the display list
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLDisplayList::call() {
		glCallList(glID);
	}

}

