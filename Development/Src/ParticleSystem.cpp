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
	//Desc:    creates new ParticleSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ParticleSystem::ParticleSystem(const String &path, int numParticles) {
		texture = GLTexture::create2d(path);

		particleList = GLDisplayList::create();
		particleList->beginBuild();
		glBegin(GL_QUADS);//Nick:TODO:Replace
		glTexCoord2f(0, 0);//Nick:TODO:Replace
		glVertex3f(-1, -1, 0);//Nick:TODO:Replace

		glTexCoord2f(1, 0);//Nick:TODO:Replace
		glVertex3f(1, -1, 0);//Nick:TODO:Replace

		glTexCoord2f(1, 1);//Nick:TODO:Replace
		glVertex3f(1, 1, 0);//Nick:TODO:Replace

		glTexCoord2f(0, 1);//Nick:TODO:Replace
		glVertex3f(-1, 1, 0);//Nick:TODO:Replace
		glEnd();//Nick:TODO:Replace
		particleList->endBuild();

		position = Vec3(0, 0, 0);
		color = Vec3(1, 1, 1);

		lifeTime = 5000;
		dispersion = 0.5;
		this->numParticles = numParticles;
		particles = new Particle[numParticles];

		for(int i = 0; i < numParticles; i++) {
			particles[i].lifeTime = 0;
			particles[i].velocity = velocity = Vec3(0, 10, 0);
			particles[i].position = Vec3(0, 0, 0);
		}
		GetEngine()->scene->addParticleSystem(this);
	}

	//---------------------------------------------------------------------------
	//Desc:    ParticleSystem destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ParticleSystem::~ParticleSystem() {
	}

	//---------------------------------------------------------------------------
	//Desc:    draw ParticleSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void ParticleSystem::draw() {
		texture->set(0);
		GetEngine()->iRender->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetEngine()->iRender->setColor(color);
		GetEngine()->iRender->depthMask(false);

		//srand(GetEngine()->iWindow->getDTime());
		float vlength = velocity.length();
		for(int i = 0; i < numParticles; i++) {

			if(lifeTime < particles[i].lifeTime || particles[i].lifeTime == 0) {
				particles[i].position = position;

				particles[i].velocity.x = velocity.x + sinf(rand()) * dispersion * vlength;
				particles[i].velocity.y = velocity.y + sinf(rand()) * dispersion * vlength; 
				particles[i].velocity.z = velocity.z + sinf(rand()) * dispersion * vlength;

				particles[i].lifeTime = ((float)i/(float)numParticles) * lifeTime;
			}

			particles[i].velocity += force * GetEngine()->iWindow->getDTime() * 0.001;
			particles[i].position += particles[i].velocity * GetEngine()->iWindow->getDTime() * 0.001;

			GetEngine()->iRender->push();
			GetEngine()->iRender->translate(particles[i].position);
			GetEngine()->iRender->scale(Vec3(5, 5, 5));

			GetEngine()->iRender->rotate(GetEngine()->scene->camera->angle[0] - 180, Vec3(0, 1, 0));
			GetEngine()->iRender->rotate(GetEngine()->scene->camera->angle[1], Vec3(1, 0, 0));

			particleList->call();

			GetEngine()->iRender->pop();

			particles[i].lifeTime += GetEngine()->iWindow->getDTime();
		}
		GetEngine()->iRender->depthMask(true);
		GetEngine()->iRender->disableBlending();
		texture->unset(0);
		GetEngine()->iRender->setColor(Vec4(1, 1, 1, 1));
	}

}