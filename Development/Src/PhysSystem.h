/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/
#pragma once
 
//***************************************************************************
#include "Core/IncludesAndLibs.h"
//***************************************************************************
#include "PhysBody.h"
#include "PhysJoint.h"
#include "MathLib.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************

namespace NGEngine {

//---------------------------------------------------------------------------
//Desc: Engine`s main physics system. Created one time
//---------------------------------------------------------------------------
class PhysSystem {
public:
	PhysSystem();

	~PhysSystem();
	void initialise();
	void update(float dTime);
	void setWorldSize(const Vec3 &worldSize);

	PhysBody *intersectWorldByRay(const Vec3 &src, const Vec3 &dst, Vec3 &normal, Vec3 &point);

private:
	NewtonWorld *nWorld;

	Vec3 worldSize;

	float accTimeSlice;

	int defaultID;

	PhysBody *pBody0;
	PhysBody *pBody1;
	float impactSpeed;
	Vec3 impactNormal, impactPosition;

	static int contactBegin(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
	static int contactProcess(const NewtonMaterial* material, const NewtonContact* contact);
	static void contactEnd(const NewtonMaterial* material);
	
	float intersectionParam;
	Vec3 intersectionNormal;
	PhysBody *intersectedBody;
	static float rayCastFilter(const NewtonBody* body, const float* normal, int collisionID, void* userData, float iParam);

	friend class PhysBody;
	friend class PhysJoint;
	friend class PhysJointUpVector;
};

};