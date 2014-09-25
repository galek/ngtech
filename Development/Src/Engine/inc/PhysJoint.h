/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//***************************************************************************
#include "../../Core/inc/IncludesAndLibs.h"
//***************************************************************************
#include "PhysBody.h"
#include "../../Core/inc/MathLib.h"
//**************************************

namespace NGTech {
	/**
	Class of the up vector joint
	*/
	class ENGINE_API PhysJointUpVector {
	public:
		explicit PhysJointUpVector(const Vec3 &direction, PhysBody *body);
		~PhysJointUpVector();
	};
}