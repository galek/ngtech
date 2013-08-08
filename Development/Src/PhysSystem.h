/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#pragma once

//***************************************************************************
#include "Core\IncludesAndLibs.h"
//***************************************************************************
#include "PhysBody.h"
#include "PhysJoint.h"
#include "MathLib.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************
struct NewtonWorld;
struct NewtonJoint;
struct NewtonMaterial;
struct NewtonBody;
struct NewtonCollision;

namespace VEGA {

/**
Engine`s main physics system. Created one time
*/
	class VEGA_API PhysSystem {
	public:
		PhysSystem();
		~PhysSystem();

		void initialise();
	/**
	Updates PhysSystem engine
	\param dTime time from the last frame in miliseconds
	*/
		void update(float dTime);

		PhysBody *intersectWorldByRay(const Vec3 &src, const Vec3 &dst, Vec3 &normal, Vec3 &point);

	private:
		NewtonWorld *nWorld;

		float accTimeSlice;

		int defaultID;

		PhysBody *pBody0;
		PhysBody *pBody1;
		float impactSpeed;
		Vec3 impactNormal, impactPosition;

		static void contactProcess(const NewtonJoint *pContactJoint, float fTimeStep, int ThreadIndex);
		static int	playContantSound(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex);

		float intersectionParam;
		Vec3 intersectionNormal;
		PhysBody *intersectedBody;
		static float rayCastFilter(const NewtonBody* const body, const NewtonCollision* const shapeHit, const float* const hitContact, const float* const hitNormal, int* const collisionID, void* const userData, float intersectParam);

		friend class PhysBody;
		friend class PhysJoint;
		friend class PhysJointUpVector;
	};

};