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
#include "MathLib.h"
//**************************************
struct NewtonJoint;

namespace NGTech {

/**
Class of the phys joint
*/
class VEGA_API PhysJoint {

	};


/**
Class of the up vector joint
*/
class VEGA_API PhysJointUpVector : public PhysJoint {
	public:
		PhysJointUpVector(const Vec3 &direction, PhysBody *body);
		~PhysJointUpVector();

	private:
		NewtonJoint *nJoint;
	};

};