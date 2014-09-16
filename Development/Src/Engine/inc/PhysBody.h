#pragma once

//***************************************************************************
#include "../../Core/inc/IncludesAndLibs.h"
//***************************************************************************
#include "../../Core/inc/MathLib.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************
namespace physx
{
	class PxShape;
	class PxRigidActor;
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

		static PhysBody *CreateConvexHull(int _numVert, int _numFaces, Mat4 *_trans, void*_vertices, unsigned int*_indices, float);
		static PhysBody *CreateStaticMesh(int _numVert, int _numFaces, Mat4 *_trans, void*, unsigned int*);

		~PhysBody();

		void SetTransform(const Mat4 &trans);
		Mat4 GetTransform();

		float GetMass();
		void SetMass(float mass);

		void AddForce(const Vec3 &force);
		Vec3 GetForce();
		void AddTorque(const Vec3 &torque);

		void SetLinearVelocity(const Vec3 &velocity);
		Vec3 GetLinearVelocity();
		void SetAngularVelocity(const Vec3 &velocity);
		Vec3 GetAngularVelocity();

		ENGINE_INLINE void SetImpactSound(ALSound *snd) {
			impactSrc = ALSoundSource::create(snd);
		}

		void SetLinearDamping(float _v);
		void SetAngularDamping(float _v);
		void SetMassSpaceInertiaTensor(const Vec3&);
	private:
		ALSoundSource *impactSrc;
		physx::PxShape* mShape;
		physx::PxRigidActor *mActor;
		Vec3 mLvelocity, mAvelocity;

		float mass;

		friend class PhysSystem;
		friend class PhysJoint;
		friend class PhysJointUpVector;
	};
}