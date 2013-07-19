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

namespace VEGA {

class GLDisplayList {
public:
	static GLDisplayList *create();
	~GLDisplayList();

	void beginBuild();
	void endBuild();
	void call();

private:
	GLuint glID;
};

};