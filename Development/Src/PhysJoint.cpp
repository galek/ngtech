/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "PhysJoint.h"
#include "PhysSystem.h"
#include "PhysBody.h"
//**************************************

namespace NGEngine {

	PhysJointUpVector *PhysJointUpVector::create(const Vec3 &direction, PhysBody *body) {
		PhysJointUpVector *joint = new PhysJointUpVector();
		joint->nJoint = NewtonConstraintCreateUpVector(engine.physSystem->nWorld, direction, body->nBody);
		return joint;
	}

	PhysJointUpVector::~PhysJointUpVector() {
		NewtonDestroyJoint(engine.physSystem->nWorld, nJoint);
	}

}
