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

#include "pvd/PxVisualDebugger.h"
#include "physxprofilesdk/PxProfileZoneManager.h"
#include "pxtask/PxCudaContextManager.h"
//***************************************************************************

#define ENABLE_PVD 1
#define DEBUG_PHYSICS 1

namespace NGTech {
	using namespace physx;

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

	/**
	*/
	PhysSystem::PhysSystem() :accTimeSlice(0.0f),
		mNbThreads(1)
	{}

	/**
	*/
	void PhysSystem::initialise()
	{
		Log::writeHeader("-- PhysSystem --");
		Debug("PhysSystem::initialise");

		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!mFoundation)
			Error("PhysSystem::initialise()-PxCreateFoundation failed!", true);
		mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
		if (!mProfileZoneManager)
			Error("PxProfileZoneManager::createProfileZoneManager failed!", true);

		PxTolerancesScale scale;
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, scale, false, mProfileZoneManager);
		if (!mPhysics)
			Error("PhysSystem::initialise()-PxCreatePhysics failed!", true);

		if (!PxInitExtensions(*mPhysics))
			Error("PhysSystem::initialise()-PxInitExtensions failed!", true);

		PxCookingParams params(scale);
		params.meshWeldTolerance = 0.0f;
		params.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES | PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES | PxMeshPreprocessingFlag::eREMOVE_DUPLICATED_TRIANGLES | PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES);//NICK:WTF?!
		mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, params);
		if (!mCooking)
			Error("PxCreateCooking failed!", true);

#ifdef ENABLE_PVD
		togglePvdConnection();
#endif
		// setup default material...
		mMaterial = mPhysics->createMaterial(1.0f, 1.0f, 1.0f);
		if (!mMaterial)
			Error("createMaterial failed!", true);

		PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);

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

#if defined (DEBUG_PHYSICS)
		// *** Create Ground-Plane *** //
		PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
		PxRigidStatic* plane = mPhysics->createRigidStatic(pose);
		PxShape* shape = plane->createShape(PxPlaneGeometry(), *mMaterial);
		mScene->addActor(*plane);
#endif
	}

	/**
	*/
	PhysSystem::~PhysSystem() {
		Debug("PhysSystem::~PhysSystem()");
		mScene->fetchResults(true);
	}

	/**
	*/
	void PhysSystem::update(float dt) {
		const float mStepSize = 1.0f / 60.0f;
		mScene->simulate(mStepSize);
		mScene->fetchResults(true);
	}

	/*
	*/
	PhysBody *PhysSystem::intersectWorldByRay(const Vec3 &src, const Vec3 &dst, Vec3 &normal, Vec3 &point) {
		Debug("PhysSystem::intersectWorldByRay");
		return NULL;
	}

	/**
	*/
	void PhysSystem::togglePvdConnection() {
		Debug("PhysSystem::togglePvdConnection()");
		if (!mPhysics->getPvdConnectionManager()){
			Debug("PhysSystem::togglePvdConnection()-1");
			return;
		}
		if (mPhysics->getPvdConnectionManager()->isConnected()){
			Debug("PhysSystem::togglePvdConnection()-2");
			mPhysics->getPvdConnectionManager()->disconnect();
		}
		else
			createPvdConnection();
	}

	/**
	*/
	void PhysSystem::createPvdConnection()	{
		Debug("PhysSystem::createPvdConnection()");
		auto mCon = PxVisualDebuggerExt::createConnection(mPhysics->getPvdConnectionManager(), "127.0.0.1", 5425, 100, PxVisualDebuggerExt::getAllConnectionFlags());
		if (mCon){
			mPhysics->getVisualDebugger()->setVisualizeConstraints(true);
			mPhysics->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::Enum::eTRANSMIT_CONTACTS, true);
		}
	}

	/**
	*/
	void PhysSystem::SetGravity(const Vec3&_vec)	{
		if (mScene)
			mScene->setGravity(PxVec3(_vec.x, _vec.y, _vec.z));
	}

	/**
	*/
	Vec3 PhysSystem::GetGravity()	{
		Vec3 Mvec(0,0,0);
		if (mScene){
			PxVec3 pvec=mScene->getGravity();
			Mvec = { pvec.x, pvec.y, pvec.z };
		}
		return Mvec;
	}
}