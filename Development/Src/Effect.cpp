/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

//**************************************
#include "Effect.h"
#include "Frustum.h"
#include "Config.h"
#include "Scene.h"
#include "WindowSystem.h"
#include <math.h>
//**************************************

//---------------------------------------------------------------------------
//Desc:    creates new EffectParticleSystem
//Params:  -
//Returns: -
//---------------------------------------------------------------------------
EffectParticleSystem::EffectParticleSystem(const String &path, int numParticles) {
	texture = GLTexture::create2d(path);
	
	particleList = GLDisplayList::create();
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

	for(int i = 0; i < numParticles; i++) {
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
	GLSystem::get()->enableBlending(GLSystem::ONE, GLSystem::ONE);
	GLSystem::get()->setColor(color);
	GLSystem::get()->depthMask(false);

	for(int i = 0; i < numParticles; i++) {
	
		if(lifeTime < particles[i].lifeTime || particles[i].lifeTime == 0) {
			particles[i].position = position;
			particles[i].velocity = Vec3(velocity.x + sinf(rand()) * Vec3::length(velocity), 
				velocity.y + cosf(rand()) * Vec3::length(velocity), 
				velocity.z + sinf(rand()) * Vec3::length(velocity))
				+ force * WindowSystem::get()->getDTime() * 0.001;
			particles[i].lifeTime = i/numParticles *lifeTime;
		};
		
		particles[i].position += particles[i].velocity * WindowSystem::get()->getDTime() * 0.001;
				
		GLSystem::get()->push();
		GLSystem::get()->translate(particles[i].position);
		GLSystem::get()->scale(Vec3(5, 5, 5));

		GLSystem::get()->rotate(Scene::get()->camera->angle[0] - 180, Vec3(0, 1, 0));
		GLSystem::get()->rotate(Scene::get()->camera->angle[1], Vec3(1, 0, 0));
			
		particleList->call();

		GLSystem::get()->pop();
		
		particles[i].lifeTime += WindowSystem::get()->getDTime();
	};
	GLSystem::get()->depthMask(true);
	GLSystem::get()->disableBlending();
	texture->unset(0);
	GLSystem::get()->setColor(Vec4(1, 1, 1, 1));
};




//---------------------------------------------------------------------------
//Desc:    creates new EffectFlare
//Params:  -
//Returns: -
//---------------------------------------------------------------------------
EffectFlare::EffectFlare(const String &path) {
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
	GLSystem::get()->push();
	GLSystem::get()->translate(position);
	GLSystem::get()->scale(Vec3(radius, radius, radius));

	GLSystem::get()->rotate(Scene::get()->camera->angle[0] - 180, Vec3(0, 1, 0));
	GLSystem::get()->rotate(Scene::get()->camera->angle[1], Vec3(1, 0, 0));
		
	texture->set(0);
	GLSystem::get()->enableBlending(GLSystem::ONE, GLSystem::ONE);
	GLSystem::get()->setColor(color);
	GLSystem::get()->depthMask(false);

	flareList->call();

	GLSystem::get()->depthMask(true);
	GLSystem::get()->disableBlending();
	texture->unset(0);

	GLSystem::get()->pop();
};




