#pragma once

#include "PxPhysicsAPI.h"
#include "..\..\Core\mathlib.h"

namespace NGTech {
	__inline physx::PxTransform EngineMathToPhysX(Mat4 *_trans)
	{
		physx::PxVec3 vec(_trans->e[12], _trans->e[13], _trans->e[14]);
		Warning("x:%i,y:%i,z:%i", _trans->e[12], _trans->e[13], _trans->e[14]);
		//Nick:TODO:–азобратьс€ как переводить квартатион
		return physx::PxTransform(vec);
	}
	_inline Mat4 PhysXToEngineMath(const physx::PxTransform &_trans)
	{
		Vec3 vec(_trans.p.x, _trans.p.y, _trans.p.z);
		return Mat4::translate(vec);
	}
}