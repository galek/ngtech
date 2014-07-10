#pragma once

#include "PxPhysicsAPI.h"
#include "..\..\Core\mathlib.h"

namespace NGTech {
	_inline physx::PxTransform EngineMathToPhysX(const Mat4 &transform)
	{
		physx::PxTransform transf;
		transf.p = { transform.e[12], transform.e[13], transform.e[14] };
		Warning("x:%i,y:%i,z:%i", transform.e[12], transform.e[13], transform.e[14]);
		//Nick:TODO:–азобратьс€ как переводить квартатион
		return transf;
	}
	_inline Mat4 PhysXToEngineMath(const physx::PxTransform &_trans)
	{
		Mat4 matr;
		matr.identity();
		matr.e[12] = _trans.p.x;
		matr.e[13] = _trans.p.y;
		matr.e[14] = _trans.p.z;
		return matr;
	}
}
/*Mat4 r;

		r.e[0] = e[0]; r.e[4] = e[4]; r.e[8]  = e[8];  r.e[12] = 0;
		r.e[1] = e[1]; r.e[5] = e[5]; r.e[9]  = e[9];  r.e[13] = 0;
		r.e[2] = e[2]; r.e[6] = e[6]; r.e[10] = e[10]; r.e[14] = 0;
		r.e[3] = 0;    r.e[7] = 0;    r.e[11] = 0;     r.e[15] = 1;*/