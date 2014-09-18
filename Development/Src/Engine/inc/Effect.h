#pragma once

//**************************************
#include "MathLib.h"
#include <vector>
//**************************************

namespace NGTech {
	//---------------------------------------------------------------------------
	//Desc: class of the effect object
	//---------------------------------------------------------------------------
	class ENGINE_API Effect {
	public:
		ENGINE_INLINE virtual void draw() {};

		virtual const Vec3 &getPosition() = 0;
		ENGINE_INLINE virtual void setPosition(const Vec3 &position) {};

		virtual const Vec3 &getColor() = 0;
		ENGINE_INLINE virtual void setColor(const Vec3 &color) {};
	};

	//---------------------------------------------------------------------------
	//Desc: class of the effect particle system
	//---------------------------------------------------------------------------
	class EffectParticleSystem : public Effect {
	public:
		explicit EffectParticleSystem(const String &path, int numParticles);
		virtual ~EffectParticleSystem();

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

		I_Texture *texture;
		I_DisplayList *particleList;

		Particle *particles;
	};

	//---------------------------------------------------------------------------
	//Desc: class of the effect flare
	//---------------------------------------------------------------------------
	class ENGINE_API EffectFlare : public Effect {
	public:
		explicit EffectFlare(const String &path);
		virtual ~EffectFlare();

		virtual void draw();

		ENGINE_INLINE virtual const Vec3 &getPosition() { return position; };
		ENGINE_INLINE virtual void setPosition(const Vec3 &position) { this->position = position; };

		ENGINE_INLINE virtual const Vec3 &getColor() { return color; };
		ENGINE_INLINE virtual void setColor(const Vec3 &color) { this->color = color; };

		ENGINE_INLINE float getRadius() { return radius; };
		ENGINE_INLINE void setRadius(float radius) { this->radius = radius; };

	private:
		Vec3 position, color;

		I_Texture *texture;
		I_DisplayList *flareList;

		float radius;
	};
}