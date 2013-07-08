/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
#include "GLSystem.h"
#include "MathLib.h"
#include <vector>
//**************************************

//---------------------------------------------------------------------------
//Desc: class of the effect object
//---------------------------------------------------------------------------
class Effect {
public:
	virtual void draw() {};

	virtual const Vec3 &getPosition() { return Vec3(0, 0, 0); };
	virtual void setPosition(const Vec3 &position) {};

	virtual const Vec3 &getColor() { return Vec3(0, 0, 0); };
	virtual void setColor(const Vec3 &color) {};
};

//---------------------------------------------------------------------------
//Desc: class of the effect particle system
//---------------------------------------------------------------------------
class EffectParticleSystem : public Effect {
public:
	EffectParticleSystem(const String &path, int numParticles);
	~EffectParticleSystem();

	virtual void draw();
	
	virtual const Vec3 &getPosition() { return position; };
	virtual void setPosition(const Vec3 &position) { this->position = position; };

	virtual const Vec3 &getColor() { return color; };
	virtual void setColor(const Vec3 &color) { this->color = color; };

	void setParticleLifeTime(int time) { lifeTime = time; };
	int getParticleLifeTime() { return lifeTime; }; 

	void setVelocity(const Vec3 &velocity) { this->velocity = velocity; };
	const Vec3 &getVelocity() { return velocity; };

	void setForce(const Vec3 &force) { this->force = force; };
	const Vec3 &getForce() { return force; };

private:
	struct Particle {
		Vec3 position;
		Vec3 velocity;
		int lifeTime;
	};

	Vec3 position, color;
	Vec3 force, velocity;

	int lifeTime;

	int numParticles;
	
	GLTexture *texture;
	GLDisplayList *particleList;

	Particle *particles;
};

//---------------------------------------------------------------------------
//Desc: class of the effect flare
//---------------------------------------------------------------------------
class EffectFlare : public Effect {
public:
	EffectFlare(const String &path);
	~EffectFlare();

	virtual void draw();
	
	virtual const Vec3 &getPosition() { return position; };
	virtual void setPosition(const Vec3 &position) { this->position = position; };

	virtual const Vec3 &getColor() { return color; };
	virtual void setColor(const Vec3 &color) { this->color = color; };

	float getRadius() { return radius; };
	void setRadius(float radius) { this->radius = radius; };

private:
	Vec3 position, color;
	
	GLTexture *texture;
	GLDisplayList *flareList;

	float radius;
};

