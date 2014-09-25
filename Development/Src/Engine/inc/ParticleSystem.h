#pragma once

//**************************************
#include "../../Core/inc/MathLib.h"
#include <vector>
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: class of the system particle system
	//---------------------------------------------------------------------------
	class ENGINE_API ParticleSystem {
	public:
		explicit ParticleSystem(const String &path, int numParticles);
		~ParticleSystem();

		virtual void draw();

		ENGINE_INLINE virtual const Vec3 &getPosition() { return position; };
		ENGINE_INLINE virtual void setPosition(const Vec3 &position) { this->position = position; };

		ENGINE_INLINE virtual const Vec3 &getColor() { return color; };
		ENGINE_INLINE virtual void setColor(const Vec3 &color) { this->color = color; };

		ENGINE_INLINE void setParticleLifeTime(int time) { lifeTime = time; };
		ENGINE_INLINE int getParticleLifeTime() { return lifeTime; };

		ENGINE_INLINE void setVelocity(const Vec3 &velocity) { this->velocity = velocity; };
		ENGINE_INLINE const Vec3 &getVelocity() { return velocity; };

		ENGINE_INLINE void setForce(const Vec3 &force) { this->force = force; };
		ENGINE_INLINE const Vec3 &getForce() { return force; };

		ENGINE_INLINE void setDispersion(float dispersion) { this->dispersion = dispersion; };
		ENGINE_INLINE float getDispersion() { return dispersion; };

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

		I_Texture *texture;
		I_DisplayList *particleList;

		Particle *particles;
	};
}