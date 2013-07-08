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
#include "GLOcclusionQuery.h"
//***************************************************************************

namespace NGEngine {

	//---------------------------------------------------------------------------
	//Desc:    creates GLOcclusionQuery
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLOcclusionQuery *GLOcclusionQuery::create() {
		GLOcclusionQuery *ocq = new GLOcclusionQuery();

		glGenQueriesARB(1, &ocq->glID);
		return ocq;
	}

	//---------------------------------------------------------------------------
	//Desc:    GLOcclusionQuery destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLOcclusionQuery::~GLOcclusionQuery() {
		glDeleteQueriesARB(1, &glID);
	}

	//---------------------------------------------------------------------------
	//Desc:    begins rendering to query
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLOcclusionQuery::beginRendering() {
		glBeginQueryARB(GL_SAMPLES_PASSED_ARB, glID);
	}

	//---------------------------------------------------------------------------
	//Desc:    end rendering to query
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLOcclusionQuery::endRendering() {
		glEndQueryARB(GL_SAMPLES_PASSED_ARB);
	}

	//---------------------------------------------------------------------------
	//Desc:    get number of passed samples
	//Params:  number of passed samples
	//Returns: -
	//---------------------------------------------------------------------------
	unsigned int GLOcclusionQuery::getResult() {
		unsigned int fc;
		glGetQueryObjectuivARB(glID, GL_QUERY_RESULT_ARB, &fc);
		return fc;
	}

}




