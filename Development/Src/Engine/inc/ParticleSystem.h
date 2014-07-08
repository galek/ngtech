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

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: class of the system particle system
	//---------------------------------------------------------------------------
	class ENGINE_API ParticleSystem {
	public:
		ParticleSystem(const String &path, int numParticles);
		~ParticleSystem();

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

		void setDispersion(float dispersion) { this->dispersion = dispersion; };
		float getDispersion() { return dispersion; };

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
		float dispersion;

		GLTexture *texture;
		GLDisplayList *particleList;

		Particle *particles;
	};
}