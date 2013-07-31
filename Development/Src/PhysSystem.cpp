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
#include "../Externals/newton/coreLibrary_200/source/newton/newton.h"
//***************************************************************************

namespace VEGA {

	//contact-callback------------------
	int PhysSystem::playContantSound(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex) {
		GetEngine()->physSystem->pBody0 = (PhysBody*) NewtonBodyGetUserData(body0);
		GetEngine()->physSystem->pBody1 = (PhysBody*) NewtonBodyGetUserData(body1);

		if (GetEngine()->physSystem->impactSpeed > 15) {
			if (GetEngine()->physSystem->pBody0) {
				if (GetEngine()->physSystem->pBody0->impactSrc) {
					if (!GetEngine()->physSystem->pBody0->impactSrc->isPlaying()) {
						GetEngine()->physSystem->pBody0->impactSrc->setPosition(GetEngine()->physSystem->impactPosition);
						GetEngine()->physSystem->pBody0->impactSrc->play();
					}
				}
			}

			if (GetEngine()->physSystem->pBody1) {
				if (GetEngine()->physSystem->pBody1->impactSrc) {
					if (!GetEngine()->physSystem->pBody1->impactSrc->isPlaying()) {
						GetEngine()->physSystem->pBody1->impactSrc->setPosition(GetEngine()->physSystem->impactPosition);
						GetEngine()->physSystem->pBody1->impactSrc->play();
					}
				}
			}
		}
		GetEngine()->physSystem->impactSpeed = 0;
		return 1;
	}
	//-----------------------------------------
	void PhysSystem::contactProcess(const NewtonJoint *pContactJoint, float fTimeStep, int ThreadIndex)
	{
		// Get pointer to body
		NewtonBody* const body0 = NewtonJointGetBody0(pContactJoint);
		for (void* contact = NewtonContactJointGetFirstContact(pContactJoint); contact; contact = NewtonContactJointGetNextContact(pContactJoint, contact)) {

				NewtonMaterial* material = NewtonContactGetMaterial(contact);
				float speed = NewtonMaterialGetContactNormalSpeed(material);
				// play sound base of the contact speed.
				//
				if (speed > GetEngine()->physSystem->impactSpeed) {
					GetEngine()->physSystem->impactSpeed = speed;
					NewtonMaterialGetContactPositionAndNormal(material, body0, GetEngine()->physSystem->impactPosition, GetEngine()->physSystem->impactNormal);
				}
			}
	}

	//---------------------------------------------------------------------------
	//Desc:    creates new PhysSystem
	//Params:  -
	//Returns: pointer to new PhysSystem
	//---------------------------------------------------------------------------
	PhysSystem::PhysSystem() : nWorld(nullptr) {}
	//---------------------------------------------------------------------------
	//Desc:    initialises new PhysSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void PhysSystem::initialise()
	{
		Log::writeHeader("-- PhysSystem --");

		nWorld = NewtonCreate();

		worldSize = Vec3(10000.0f, 10000.0f, 10000.0f);
		NewtonSetWorldSize(nWorld, -worldSize * 0.5f, worldSize * 0.5f);

		accTimeSlice = 0.0f;

		defaultID = NewtonMaterialGetDefaultGroupID(nWorld);

		NewtonMaterialSetDefaultSoftness(nWorld, defaultID, defaultID, 0.05f);
		NewtonMaterialSetDefaultElasticity(nWorld, defaultID, defaultID, 0.4f);
		NewtonMaterialSetDefaultCollidable(nWorld, defaultID, defaultID, 1);
		NewtonMaterialSetDefaultFriction(nWorld, defaultID, defaultID, 1.0f, 0.5f);
		NewtonMaterialSetCollisionCallback(nWorld, defaultID, defaultID, NULL, playContantSound, contactProcess);
	}

	//---------------------------------------------------------------------------
	//Desc:    PhysSystem destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	PhysSystem::~PhysSystem() {
		NewtonDestroy(nWorld);
	}

	//---------------------------------------------------------------------------
	//Desc:    updates PhysSystem engine
	//Params:  dTime - time from the last frame in miliseconds
	//Returns: -
	//---------------------------------------------------------------------------
	void PhysSystem::update(float dTime) {
		accTimeSlice += dTime;

		while (accTimeSlice > 12.0f) {
			NewtonUpdate(nWorld, (12.0f / 1000.0f));
			accTimeSlice -= 12.0f;
		}

		intersectionParam = 100000.0;
	}

	//---------------------------------------------------------------------------
	//Desc:    sets PhysSystem world size
	//Params:  worldSize - world size 
	//Returns: -
	//---------------------------------------------------------------------------
	void PhysSystem::setWorldSize(const Vec3 &worldSize) {
		this->worldSize = worldSize;
		NewtonSetWorldSize(nWorld, -worldSize * 0.5, worldSize * 0.5);
	}

	//---------------------------------------------------------------------------
	//Desc:    Newton callback
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	float PhysSystem::rayCastFilter(const NewtonBody* body, const float* normal, int collisionID, void* userData, float iParam) {
		if (iParam < GetEngine()->physSystem->intersectionParam) {
			GetEngine()->physSystem->intersectionParam = iParam;
			GetEngine()->physSystem->intersectionNormal = Vec3(normal[0], normal[1], normal[2]);
			GetEngine()->physSystem->intersectedBody = (PhysBody*) NewtonBodyGetUserData(body);
		}
		return iParam;
	}

	//---------------------------------------------------------------------------
	//Desc:    sets PhysSystems world by ray
	//Params:  src - ray src, dst 
	//Returns: -
	//---------------------------------------------------------------------------
	PhysBody *PhysSystem::intersectWorldByRay(const Vec3 &src, const Vec3 &dst, Vec3 &normal, Vec3 &point) {
		NewtonWorldRayCast(nWorld, src, dst, rayCastFilter, NULL, NULL);

		if (intersectedBody) {
			point = src + (dst - src) * intersectionParam;
			normal = intersectionNormal;
			return intersectedBody;
		}
		return NULL;
	}

}