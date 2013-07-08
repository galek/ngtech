/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//************************************
#include "Engine.h"
#include "Water.h"
#include "WindowSystem.h"
//************************************

namespace NGEngine {

	Water::Water() {
		size = 500;
		depth = 10;
	}

	Water::~Water() {
	}

	void Water::draw() {
		engine.iRender->disableCulling();

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-size, depth, -size);

		glTexCoord2f(0, 1);
		glVertex3f(-size, depth, size);

		glTexCoord2f(1, 1);
		glVertex3f(size, depth, size);

		glTexCoord2f(1, 0);
		glVertex3f(size, depth, -size);
		glEnd();

		engine.iRender->enableCulling();
	}

}