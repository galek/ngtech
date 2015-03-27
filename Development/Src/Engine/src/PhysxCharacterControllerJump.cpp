#include "EnginePrivate.h"
//***************************************************************************
#include "PxPhysicsAPI.h"
//***************************************************************************
#include "PhysSystem.h"
#include "PhysBody.h"
#include "Log.h"
//***************************************************************************
#include "EngineMathToPhysx.inl"
#include "PhysXCharacterController.h"
//***************************************************************************

namespace NGTech
{
	using namespace physx;

	static float gJumpGravity = 0.005f;

	PhysXCharacterController::JumpAction::JumpAction() :
		mV0(0.0f),
		mJumpTime(0.0f),
		mJump(false)
	{
	}

	void PhysXCharacterController::JumpAction::StartJump(float v0)
	{
		if (mJump)	return;
		mJumpTime = 0.0f;
		mV0 = v0;
		mJump = true;
	}

	void PhysXCharacterController::JumpAction::StopJump()
	{
		if (!mJump)	return;
		mJump = false;
		mJumpTime = 0.0f;
		mV0 = 0.0f;
	}

	float PhysXCharacterController::JumpAction::GetHeight(float elapsedTime, float _descHeight)
	{
		if (!mJump)	return 0.0f;
		mJumpTime += elapsedTime;

		const float h = _descHeight;//Nick:TODO:Зафиксировал прыжок,но он короткосрочный
		if (h <= 0)
			Warning("In: %s:%i height: %f of jump <0,it's correct?", __FUNCTION__, __LINE__, h);
		return h*elapsedTime;
	}


}