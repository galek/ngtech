#include "RenderPrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLOcclusionQuery.h"
//***************************************************************************

namespace NGTech {
	/*
	*/
	GLOcclusionQuery::GLOcclusionQuery() {
		glGenQueriesARB(1, &glID);
	}

	/*
	*/
	GLOcclusionQuery::~GLOcclusionQuery() {
		glDeleteQueriesARB(1, &glID);
	}

	/*
	*/
	void GLOcclusionQuery::beginRendering() {
		glBeginQueryARB(GL_SAMPLES_PASSED_ARB, glID);
	}

	/*
	*/
	void GLOcclusionQuery::endRendering() {
		glEndQueryARB(GL_SAMPLES_PASSED_ARB);
	}

	/*
	*/
	unsigned int GLOcclusionQuery::getResult() {
		unsigned int fc;
		glGetQueryObjectuivARB(glID, GL_QUERY_RESULT_ARB, &fc);
		return fc;
	}

}