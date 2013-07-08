/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//***************************************************************************
#include "Core/IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGEngine {

class GLOcclusionQuery {
public:
	static GLOcclusionQuery *create();
	~GLOcclusionQuery();

	void beginRendering();
	void endRendering();

	unsigned int getResult();

private:
	GLuint glID;
};

};