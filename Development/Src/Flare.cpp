/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    Flare.cpp
* Desc:    Lights Flare impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/

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

		flareList = new GLDisplayList();
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
		radius = 5.0f;
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
		GetEngine()->iRender->push();
		GetEngine()->iRender->translate(position);
		GetEngine()->iRender->scale(Vec3(radius, radius, radius));

		GetEngine()->iRender->rotate(GetEngine()->scene->camera->angle[0] - 180, Vec3(0, 1, 0));
		GetEngine()->iRender->rotate(GetEngine()->scene->camera->angle[1], Vec3(1, 0, 0));

		texture->set(0);
		GetEngine()->iRender->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetEngine()->iRender->setColor(color);
		GetEngine()->iRender->depthMask(false);

		flareList->call();

		GetEngine()->iRender->depthMask(true);
		GetEngine()->iRender->disableBlending();
		texture->unset(0);

		GetEngine()->iRender->pop();
	}

}




