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

namespace NGEngine {



	//contact-callback------------------
	int PhysSystem::contactBegin(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1) {
		Debug("contactBegin");
		if(engine.physSystem)
		{
			engine.physSystem->pBody0 = (PhysBody*)NewtonBodyGetUserData(body0);
			engine.physSystem->pBody1 = (PhysBody*)NewtonBodyGetUserData(body1);

			engine.physSystem->impactSpeed = 0;
		}
		return 1;
	}

	//contact-callback------------------
	int PhysSystem::contactProcess(const NewtonMaterial* material, const NewtonContact* contact) {
		Debug("contactProcess");
		float speed;
		if(engine.physSystem)
		{
		speed = NewtonMaterialGetContactNormalSpeed(material, contact);
		if(speed > engine.physSystem->impactSpeed) {
			engine.physSystem->impactSpeed = speed;
			NewtonMaterialGetContactPositionAndNormal(material, engine.physSystem->impactPosition, engine.physSystem->impactNormal);
		}
		}
		return 1;	
	}	

	//contact-callback------------------
	void PhysSystem::contactEnd(const NewtonMaterial* material) {
		Debug("contactEnd");
		if(engine.physSystem)
		{
			if(engine.physSystem->impactSpeed > 15) {
				if(engine.physSystem->pBody0) { 
					if(engine.physSystem->pBody0->impactSrc) {
						if(!engine.physSystem->pBody0->impactSrc->isPlaying()) {
							engine.physSystem->pBody0->impactSrc->setPosition(engine.physSystem->impactPosition);
							engine.physSystem->pBody0->impactSrc->play();
						}
					} 
				}

				if(engine.physSystem->pBody1) { 
					if(engine.physSystem->pBody1->impactSrc) {
						if(!engine.physSystem->pBody1->impactSrc->isPlaying()) {
							engine.physSystem->pBody1->impactSrc->setPosition(engine.physSystem->impactPosition);
							engine.physSystem->pBody1->impactSrc->play();
						}
					} 
				}
			}
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    creates new PhysSystem
	//Params:  -
	//Returns: pointer to new PhysSystem
	//---------------------------------------------------------------------------
	PhysSystem::PhysSystem():nWorld(nullptr){
		Log::write("PhysSystem::create()");
		Log::write("{");
		Log::write("}\n");

		nWorld = NewtonCreate(NULL, NULL);
		if(!nWorld)
			return;

		worldSize = Vec3(10000, 10000, 10000);
		NewtonSetWorldSize(nWorld, worldSize * 0.5, worldSize * 0.5);

		accTimeSlice = 0.0f;

		defaultID = NewtonMaterialGetDefaultGroupID(nWorld);

		NewtonMaterialSetDefaultSoftness(nWorld, defaultID, defaultID, 0.05f);
		NewtonMaterialSetDefaultElasticity(nWorld, defaultID, defaultID, 0.4f);
		NewtonMaterialSetDefaultCollidable(nWorld, defaultID, defaultID, 1);
		NewtonMaterialSetDefaultFriction(nWorld, defaultID, defaultID, 1.0f, 0.5f);
		NewtonMaterialSetCollisionCallback(nWorld, defaultID, defaultID, NULL, contactBegin, contactProcess, contactEnd); 

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

		while(accTimeSlice > 12.0f) {
			if(nWorld)	NewtonUpdate(nWorld, (12.0f / 1000.0f));
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
		if(engine.physSystem)
		{
			if(iParam < engine.physSystem->intersectionParam) {
			engine.physSystem->intersectionParam = iParam;
			engine.physSystem->intersectionNormal = Vec3(normal[0],normal[1],normal[2]);
			engine.physSystem->intersectedBody = (PhysBody*)NewtonBodyGetUserData(body);
			}
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

		if(intersectedBody) {
			point = src + (dst - src) * intersectionParam;
			normal = intersectionNormal;
			return intersectedBody;
		}
		return NULL;
	}

}