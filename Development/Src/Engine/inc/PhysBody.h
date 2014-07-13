#pragma once

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************
namespace physx
{
	class PxShape;
	class PxRigidDynamic;
}
namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Rigid body struct
	//---------------------------------------------------------------------------
	class PhysBody {
	public:
		static PhysBody *CreateBox(const Vec3 &size, Mat4 *_trans, float mass = 0);
		static PhysBody *CreateSphere(float radius, Mat4 *_trans, float mass = 0);

		static PhysBody *CreateCylinder(float radius, float height, Mat4 *_trans, float mass = 0);
		static PhysBody *CreateCone(float radius, float height, float mass = 0);
		static PhysBody *CreateCapsule(float radius, float height, Mat4 *_trans, float mass = 0);
		static PhysBody *CreateChampferCylinder(float radius, float height, float mass = 0);

		static PhysBody *CreateConvexHull(Vec3 *pos, const int numPos, float mass = 0);
		static PhysBody *CreateStaticMesh(Vec3 *pos, const int numPos, bool optimize);

		~PhysBody();

		void SetTransform(const Mat4 &trans);
		Mat4 GetTransform();

		float GetMass();
		void SetMass(float mass);

		void AddForce(const Vec3 &force);
		Vec3 GetForce();
		void AddTorque(const Vec3 &torque);
		Vec3 GetTorque();

		void AddVelocity(const Vec3 &velocity);
		void SetLinearVelocity(const Vec3 &velocity);
		Vec3 GetVelocity();

		typedef void(*ContactCallback)();

		void SetImpactSound(ALSound *snd) {
			impactSrc = ALSoundSource::create(snd);
		}
		
		void SetLinearDamping(float _v);
		void SetAngularDamping(float _v);
		void SetMassSpaceInertiaTensor(const Vec3&);
	private:
		ALSoundSource *impactSrc;
		physx::PxShape* mShape;
		physx::PxRigidDynamic *mActor;
		Vec3 force;
		Vec3 torque;
		Vec3 impulse;
		Vec3 velocity;

		float mass;

		bool cc;

		friend class PhysSystem;
		friend class PhysJoint;
		friend class PhysJointUpVector;
	};
}