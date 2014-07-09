/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//***************************************************************************
#include "Engine.h"
#include "PhysSystem.h"
#include "PhysBody.h"
#include "Log.h"
#include "Config.h"
//***************************************************************************

#include "PxPhysicsAPI.h"
#include "PxPhysXConfig.h"
#include "PxFiltering.h"
#include "foundation/PxMemory.h"

#include "PsFile.h"

#include "extensions/PxDefaultStreams.h"

#include "ParticleSystem.h"
#include "physxprofilesdk/PxProfileZoneManager.h"
#ifdef PX_PS3
#include "extensions/ps3/PxPS3Extension.h"
#include "extensions/ps3/PxDefaultSpuDispatcher.h"
//#define PRINT_BLOCK_COUNTERS
#endif
#include "pxtask\PxCudaContextManager.h"
//***************************************************************************

namespace NGTech {
	using namespace physx;

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

	/*
	*/
	PhysSystem::PhysSystem() :accTimeSlice(0.0f),
		mNbThreads(1)
	{}

	/*
	*/
	void PhysSystem::initialise()
	{
		Log::writeHeader("-- PhysSystem --");

		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!mFoundation)
			Error("PhysSystem::initialise()-PxCreateFoundation failed!",true);
		mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
		if (!mProfileZoneManager)
			Error("PxProfileZoneManager::createProfileZoneManager failed!", true);

		PxTolerancesScale scale;
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, scale, false, mProfileZoneManager);
		if (!mPhysics)
			Error("PhysSystem::initialise()-PxCreatePhysics failed!", true);

		if (!PxInitExtensions(*mPhysics))
			Error("PhysSystem::initialise()-PxInitExtensions failed!", true);

		
		PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

		if (!sceneDesc.cpuDispatcher)
		{
			mCpuDispatcher = PxDefaultCpuDispatcherCreate(mNbThreads);
			if (!mCpuDispatcher)
				Error("PhysSystem::initialise()-PxDefaultCpuDispatcherCreate failed!", true);
			sceneDesc.cpuDispatcher = mCpuDispatcher;
		}

		if (!sceneDesc.filterShader)
			sceneDesc.filterShader = gDefaultFilterShader;


		mScene = mPhysics->createScene(sceneDesc);
		if (!mScene)
			Error("PhysSystem::initialise()-createScene failed!", true);
	}

	/*
	*/
	PhysSystem::~PhysSystem() {
	}

	/*
	*/
	void PhysSystem::update(float dTime) {
		accTimeSlice += dTime;

		const float timestep = 1.0f / 60.0f;
		while (accTimeSlice>0.0f)
		{
			const float dt = dTime >= timestep ? timestep : dTime;
			if (mScene)
				mScene->simulate(dTime);
			accTimeSlice -= dt;
		}
		intersectionParam = 100000.0;
	}

	/*
	*/
	PhysBody *PhysSystem::intersectWorldByRay(const Vec3 &src, const Vec3 &dst, Vec3 &normal, Vec3 &point) {
		return NULL;
	}
}