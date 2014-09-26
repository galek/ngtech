/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    Flare.cpp
* Desc:    Lights Flare impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
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

namespace NGTech {

	Flare::Flare(const String &path) {
		texture = GetRender()->TextureCreate2D(path);

		flareList = GetRender()->GetDL();
		flareList->beginBuild();
#if 0
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
#else
		GetRender()->drawRect(-1, -1,1,1,0,0,1,0);
#endif
		flareList->endBuild();

		position = Vec3(0, 0, 0);
		color = Vec3(1, 1, 1);
		radius = 5.0f;
	}

	Flare::~Flare() {
	}

	void Flare::draw() {
		GetRender()->push();
		GetRender()->translate(position);
		GetRender()->scale(Vec3(radius, radius, radius));

		GetRender()->rotate(GetScene()->GetActiveCamera()->GetAngle(0) - 180, Vec3(0, 1, 0));
		GetRender()->rotate(GetScene()->GetActiveCamera()->GetAngle(1), Vec3(1, 0, 0));

		texture->set(0);
		GetRender()->enableBlending(I_Render::ONE, I_Render::ONE);
		GetRender()->setColor(color);
		GetRender()->depthMask(false);

		flareList->call();

		GetRender()->depthMask(true);
		GetRender()->disableBlending();
		texture->unset(0);

		GetRender()->pop();
	}
}