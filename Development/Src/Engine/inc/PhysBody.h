/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#pragma once

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************
struct NewtonBody;

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Rigid body struct
	//---------------------------------------------------------------------------
	class PhysBody {
	public:
		static PhysBody *createBox(const Vec3 &size, float mass = 0);
		static PhysBody *createSphere(float radius, float mass = 0);

		static PhysBody *createCylinder(float radius, float height, float mass = 0);
		static PhysBody *createCone(float radius, float height, float mass = 0);
		static PhysBody *createCapsule(float radius, float height, float mass = 0);
		static PhysBody *createChampferCylinder(float radius, float height, float mass = 0);

		static PhysBody *createConvexHull(Vec3 *pos, const int numPos, float mass = 0);
		static PhysBody *createStaticMesh(Vec3 *pos, const int numPos, bool optimize);

		~PhysBody();

		void setTransform(const Mat4 &trans);
		Mat4 getTransform();

		float getMass();
		void setMass(float mass);

		void addForce(const Vec3 &force);
		Vec3 getForce();
		void addTorque(const Vec3 &torque);
		Vec3 getTorque();

		void addVelocity(const Vec3 &velocity);
		void setVelocity(const Vec3 &velocity);
		Vec3 getVelocity();

		typedef void (*ContactCallback)();

		void setImpactSound(ALSound *snd) {
			impactSrc = ALSoundSource::create(snd);
		};

	private:
		NewtonBody *nBody;

		ALSoundSource *impactSrc;

		Vec3 force;
		Vec3 torque;
		Vec3 impulse;
		Vec3 velocity;

		float mass;

		static void applyForce_Callback(const NewtonBody* body, float timestep, int threadIndex);

		ContactCallback contact;
		bool cc;

		friend class PhysSystem;
		friend class PhysJoint;
		friend class PhysJointUpVector;
	};

};
