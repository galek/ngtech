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

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc:    creates new ParticleSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ParticleSystem::ParticleSystem(const String &path, int numParticles) {
		texture = GLTexture::create2d(path);

		particleList = GetRender()->GetDL();
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
		GetScene()->addParticleSystem(this);
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
		GetRender()->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetRender()->setColor(color);
		GetRender()->depthMask(false);

		//srand(GetWindow()->getDTime());
		float vlength = velocity.length();
		for(int i = 0; i < numParticles; i++) {

			if(lifeTime < particles[i].lifeTime || particles[i].lifeTime == 0) {
				particles[i].position = position;

				particles[i].velocity.x = velocity.x + sinf(rand()) * dispersion * vlength;
				particles[i].velocity.y = velocity.y + sinf(rand()) * dispersion * vlength; 
				particles[i].velocity.z = velocity.z + sinf(rand()) * dispersion * vlength;

				particles[i].lifeTime = ((float)i/(float)numParticles) * lifeTime;
			}

			particles[i].velocity += force * GetWindow()->getDTime() * 0.001;
			particles[i].position += particles[i].velocity * GetWindow()->getDTime() * 0.001;

			GetRender()->push();
			GetRender()->translate(particles[i].position);
			GetRender()->scale(Vec3(5, 5, 5));

			GetRender()->rotate(GetScene()->camera->angle[0] - 180, Vec3(0, 1, 0));
			GetRender()->rotate(GetScene()->camera->angle[1], Vec3(1, 0, 0));

			particleList->call();

			GetRender()->pop();

			particles[i].lifeTime += GetWindow()->getDTime();
		}
		GetRender()->depthMask(true);
		GetRender()->disableBlending();
		texture->unset(0);
		GetRender()->setColor(Vec4(1, 1, 1, 1));
	}

}