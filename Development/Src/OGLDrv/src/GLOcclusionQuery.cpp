#include "RenderPrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLOcclusionQuery.h"
//***************************************************************************

namespace NGTech {
	/**
	*/
	GLOcclusionQuery::GLOcclusionQuery() {
		query_id = 0;
		result = 1;
	}

	/**
	*/
	GLOcclusionQuery::~GLOcclusionQuery() {
		if (glIsQuery(query_id)) glDeleteQueries(1, &query_id);
	}

	/**
	*/
	void GLOcclusionQuery::beginRendering() {
		if (query_id == 0) glGenQueries(1, &query_id);
		glBeginQuery(GL_ANY_SAMPLES_PASSED_CONSERVATIVE, query_id);
	}

	/**
	*/
	void GLOcclusionQuery::endRendering() {
		glEndQuery(GL_ANY_SAMPLES_PASSED_CONSERVATIVE);
	}

	/**
	*/
	unsigned int GLOcclusionQuery::getResult() {
		if (query_id == 0) return 1;
		GLint available = 0;
		glGetQueryObjectiv(query_id, GL_QUERY_RESULT_AVAILABLE, &available);
		if (available) glGetQueryObjectuiv(query_id, GL_QUERY_RESULT, &result);
		return result;
	}
	
	/**
	*/
	void GLOcclusionQuery::clear() {
		result = 1;
	}

	/**
	*/
	void GLOcclusionQuery::destroy() {
		query_id = 0;
		result = 1;
	}

}