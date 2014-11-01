/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    PhysXUtils.h
* Desc:    PhysX Utils
* Version: 1.01
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#include "EnginePrivate.h"
//***************************************************************************
#include "PhysSystem.h"
#include "PhysBody.h"
#include "Log.h"
//***************************************************************************
#include "PxPhysicsAPI.h"
#include "EngineMathToPhysx.inl"
//***************************************************************************

namespace NGTech {
	using namespace physx;

	PxConvexMesh* createConvexMesh(const PxVec3* verts, const PxU32 numVerts, PxPhysics& physics, PxCooking& cooking);
	PxConvexMesh* createCylinderConvexMesh(const PxF32 width, const PxF32 radius, const PxU32 numCirclePoints, PxPhysics& physics, PxCooking& cooking);
	void SetupDefaultRigidDynamic(PxRigidDynamic& body, bool kinematic = false);
}