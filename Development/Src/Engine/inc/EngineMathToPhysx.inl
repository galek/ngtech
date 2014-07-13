#pragma once

#include "PxPhysicsAPI.h"
#include "..\..\Core\mathlib.h"

namespace NGTech {
	__inline physx::PxTransform EngineMathToPhysX(Mat4 *_trans)
	{
		physx::PxTransform trans;

		auto r = _trans->getRotation();
		Quat q(r);
		trans.p = { _trans->e[12], _trans->e[13], _trans->e[14] };
		physx::PxQuat quat(q.x,q.y,q.z,q.w);
		trans.q = quat;
		return trans;
	}
	_inline Mat4 PhysXToEngineMath(const physx::PxTransform &_trans)
	{
		Quat quat;
		quat.x = _trans.q.x;
		quat.y = _trans.q.y;
		quat.z = _trans.q.z;
		quat.w = _trans.q.w;
		Mat4 matrix = quat.toMatrix();
		matrix.e[12] = _trans.p.x;
		matrix.e[13] = _trans.p.y;
		matrix.e[14] = _trans.p.z;
		return matrix;
	}
}