#pragma once

//***************************************************************************
#include "../../Core/inc/IncludesAndLibs.h"
//***************************************************************************
#include "PhysBody.h"
#include "PhysJoint.h"
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
	class PxControllerManager;
	class PxSceneDesc;
}
//***************************************************************************

namespace NGTech {
	/**
	*/
	class PhysicsUpdateJob;

	/**
	Engine`s main physics system. Created one time
	*/
	class ENGINE_API PhysSystem
	{
	public:
		PhysSystem(CVARManager*);
		~PhysSystem();

		void initialise();
		/**
		Updates PhysSystem engine
		\param dTime time from the last frame in miliseconds
		*/
		void update();
		void SetGravity(const Vec3&_vec);
		Vec3 GetGravity();
		PhysBody *intersectWorldByRay(const Vec3 &src, const Vec3 &dst, Vec3 &normal, Vec3 &point);
	public:
		ENGINE_INLINE physx::PxFoundation* GetPxFoundation(){ return mFoundation; }
		ENGINE_INLINE physx::PxProfileZoneManager* GetPxProfileZoneManager(){ return mProfileZoneManager; }
		ENGINE_INLINE physx::PxPhysics* GetPxPhysics(){ return mPhysics; }
		ENGINE_INLINE physx::PxCooking* GetPxCooking(){ return mCooking; }
		ENGINE_INLINE physx::PxScene* GetPxScene(){ return mScene; }
		ENGINE_INLINE physx::PxMaterial* GetPxMaterial(){ return mMaterial; }
		ENGINE_INLINE physx::PxDefaultCpuDispatcher* GetPxDefaultCpuDispatcher(){ return mCpuDispatcher; }
		ENGINE_INLINE physx::PxControllerManager* GetPxControllerManager(){ return mCCManager; }
	public:
		void runUpdate();
		void waitUpdate();
		void LockRead();
		void UnLockRead();
		void LockWrite();
		void UnLockWrite();
	private:
		void _CreateCUDADispatcher(physx::PxSceneDesc*_sceneDesc);
		void _CreateScene(const physx::PxSceneDesc*_sceneDesc);
		void _togglePvdConnection();
		void _createPvdConnection();
	private:
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
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
		physx::PxCudaContextManager*				    mCudaContextManager;
#endif
		physx::PxControllerManager*						mCCManager;
		CVARManager*									info;
		PhysicsUpdateJob*								mUpdateJob;
		int update_id;									// update identifier
	};
}