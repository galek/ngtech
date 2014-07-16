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
#include "PhysBody.h"
#include "PhysJoint.h"
#include "MathLib.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************
namespace physx
{
	class PxPhysics;
	class PxCooking;
	class PxDefaultCpuDispatcher;
	class PxFoundation;
	class PxProfileZoneManager;
	class PxScene;
	class PxMaterial;
	class PxCudaContextManager;
}
namespace NGTech {

	/**
	Engine`s main physics system. Created one time
	*/
	class ENGINE_API PhysSystem {
	public:
		PhysSystem();
		~PhysSystem();

		void initialise();
		/**
		Updates PhysSystem engine
		\param dTime time from the last frame in miliseconds
		*/
		void update(float dTime);
		void SetGravity(const Vec3&_vec);
		Vec3 GetGravity();
		PhysBody *intersectWorldByRay(const Vec3 &src, const Vec3 &dst, Vec3 &normal, Vec3 &point);
		ENGINE_INLINE physx::PxFoundation* GetPxFoundation(){ return mFoundation; }
		ENGINE_INLINE physx::PxProfileZoneManager* GetPxProfileZoneManager(){ return mProfileZoneManager; }
		ENGINE_INLINE physx::PxPhysics* GetPxPhysics(){ return mPhysics; }
		ENGINE_INLINE physx::PxCooking* GetPxCooking(){ return mCooking; }
		ENGINE_INLINE physx::PxScene* GetPxScene(){ return mScene; }
		ENGINE_INLINE physx::PxMaterial* GetPxMaterial(){ return mMaterial; }
		ENGINE_INLINE physx::PxDefaultCpuDispatcher* GetPxDefaultCpuDispatcher(){ return mCpuDispatcher; }
	private:
		void togglePvdConnection();
		void createPvdConnection();
	private:
	
		float accTimeSlice;
		
		PhysBody *pBody0;
		PhysBody *pBody1;
		float impactSpeed;
		Vec3 impactNormal, impactPosition;

		float intersectionParam;
		Vec3 intersectionNormal;
		PhysBody *intersectedBody;
		
		friend class PhysBody;
		friend class PhysJoint;
		friend class PhysJointUpVector;
	private:
		physx::PxFoundation*							mFoundation;
		physx::PxProfileZoneManager*					mProfileZoneManager;
		physx::PxPhysics*								mPhysics;
		physx::PxCooking*								mCooking;
		physx::PxScene*								    mScene;
		physx::PxMaterial*								mMaterial;
		physx::PxDefaultCpuDispatcher*					mCpuDispatcher;
		int mNbThreads;
	};
}