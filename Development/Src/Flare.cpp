/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "ParticleSystem.h"
#include "Frustum.h"
#include "Config.h"
#include "Scene.h"
#include "WindowSystem.h"
#include <math.h>
//**************************************

namespace VEGA {

	//---------------------------------------------------------------------------
	//Desc:    creates new Flare
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Flare::Flare(const String &path) {
		texture = GLTexture::create2d(path);

		flareList = GLDisplayList::create();
		flareList->beginBuild();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);
		glEnd();
		flareList->endBuild();

		position = Vec3(0, 0, 0);
		color = Vec3(1, 1, 1);
		radius = 5.0;
	}

	//---------------------------------------------------------------------------
	//Desc:    Flare destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Flare::~Flare() {
	}

	//---------------------------------------------------------------------------
	//Desc:    draw Flare
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void Flare::draw() {
		engine.iRender->push();
		engine.iRender->translate(position);
		engine.iRender->scale(Vec3(radius, radius, radius));

		engine.iRender->rotate(engine.scene->camera->angle[0] - 180, Vec3(0, 1, 0));
		engine.iRender->rotate(engine.scene->camera->angle[1], Vec3(1, 0, 0));

		texture->set(0);
		engine.iRender->enableBlending(GLSystem::ONE, GLSystem::ONE);
		engine.iRender->setColor(color);
		engine.iRender->depthMask(false);

		flareList->call();

		engine.iRender->depthMask(true);
		engine.iRender->disableBlending();
		texture->unset(0);

		engine.iRender->pop();
	}

}




