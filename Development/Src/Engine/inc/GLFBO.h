/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

class GLFBO {
public:
	static GLFBO *create(int width, int height);
	void createColorAttachment();
	void createDepthAttachment();
	void createStencilAttachment();

	~GLFBO();

	void setColorTarget(GLTexture *texture = NULL, int face = -1);
	void setDepthTarget(GLTexture *texture = NULL);

	void set();
	void unset();

	void clear();
	void flush();

private:
	GLuint glID;

	GLuint glColorID;
	GLuint glStencilID;
	GLuint glDepthID;

	GLTexture *colorTarget;
	GLTexture *depthTarget;

	int width, height;
};

};