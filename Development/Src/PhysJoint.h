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
class NewtonJoint;

namespace VEGA {

	//---------------------------------------------------------------------------
	//Desc: class of the phys joint
	//---------------------------------------------------------------------------
	class PhysJoint {

	};


	//---------------------------------------------------------------------------
	//Desc: class of the up vector joint
	//---------------------------------------------------------------------------
	class PhysJointUpVector : public PhysJoint {
	public:
		static PhysJointUpVector *create(const Vec3 &direction, PhysBody *body);
		~PhysJointUpVector();

	private:
		NewtonJoint *nJoint;
	};

};