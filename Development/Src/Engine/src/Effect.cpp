/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/
#include "EnginePrivate.h"
#include "Engine.h"
//**************************************
#include "Effect.h"
#include "Frustum.h"
#include "Config.h"
#include "Scene.h"
#include "WindowSystem.h"
#include <math.h>
//**************************************

namespace NGTech {
	//---------------------------------------------------------------------------
	//Desc:    creates new EffectParticleSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	EffectParticleSystem::EffectParticleSystem(const String &path, int numParticles) {
		texture = GLTexture::create2d(path);

		particleList = new GLDisplayList();
		particleList->beginBuild();
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
		particleList->endBuild();

		position = Vec3(0, 0, 0);
		color = Vec3(1, 1, 1);

		lifeTime = 5000;
		this->numParticles = numParticles;
		particles = new Particle[numParticles];

		for (int i = 0; i < numParticles; i++) {
			particles[i].lifeTime = 0;
			particles[i].velocity = velocity = Vec3(0, 10, 0);
			particles[i].position = Vec3(0, 0, 0);
		};
	};

	//---------------------------------------------------------------------------
	//Desc:    EffectParticleSystem destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	EffectParticleSystem::~EffectParticleSystem() {
	};

	//---------------------------------------------------------------------------
	//Desc:    draw EffectParticleSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void EffectParticleSystem::draw() {
		texture->set(0);
		GetRender()->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetRender()->setColor(color);
		GetRender()->depthMask(false);

		for (int i = 0; i < numParticles; i++) {

			if (lifeTime < particles[i].lifeTime || particles[i].lifeTime == 0) {
				particles[i].position = position;
				particles[i].velocity = Vec3(velocity.x + sinf(rand()) * velocity.length(),
					velocity.y + cosf(rand()) * velocity.length(),
					velocity.z + sinf(rand()) * velocity.length())
					+ force * GetWindow()->getDTime() * 0.001;
				particles[i].lifeTime = i / numParticles *lifeTime;
			};

			particles[i].position += particles[i].velocity * GetWindow()->getDTime() * 0.001;

			GetRender()->push();
			GetRender()->translate(particles[i].position);
			GetRender()->scale(Vec3(5, 5, 5));

			GetRender()->rotate(GetScene()->GetActiveCamera()->GetAngle(0) - 180, Vec3(0, 1, 0));
			GetRender()->rotate(GetScene()->GetActiveCamera()->GetAngle(1), Vec3(1, 0, 0));

			particleList->call();

			GetRender()->pop();

			particles[i].lifeTime += GetWindow()->getDTime();
		};
		GetRender()->depthMask(true);
		GetRender()->disableBlending();
		texture->unset(0);
		GetRender()->setColor(Vec4(1, 1, 1, 1));
	};




	//---------------------------------------------------------------------------
	//Desc:    creates new EffectFlare
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	EffectFlare::EffectFlare(const String &path) {
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
		radius = 5.0;
	};

	//---------------------------------------------------------------------------
	//Desc:    EffectFlare destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	EffectFlare::~EffectFlare() {
	};

	//---------------------------------------------------------------------------
	//Desc:    draw EffectFlare
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void EffectFlare::draw() {
		GetRender()->push();
		GetRender()->translate(position);
		GetRender()->scale(Vec3(radius, radius, radius));

		GetRender()->rotate(GetScene()->GetActiveCamera()->GetAngle(0) - 180, Vec3(0, 1, 0));
		GetRender()->rotate(GetScene()->GetActiveCamera()->GetAngle(1), Vec3(1, 0, 0));

		texture->set(0);
		GetRender()->enableBlending(GLSystem::ONE, GLSystem::ONE);
		GetRender()->setColor(color);
		GetRender()->depthMask(false);

		flareList->call();

		GetRender()->depthMask(true);
		GetRender()->disableBlending();
		texture->unset(0);

		GetRender()->pop();
	};
}