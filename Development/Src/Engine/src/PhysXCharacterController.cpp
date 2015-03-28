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
	/**
	*/
	using namespace physx;

	/**
	*/
	CharacterControllerDesc::CharacterControllerDesc(Mode _mMode)
	{
		mMode = _mMode;
		radius = 0.0f;
		height = 0.0f;
		density = 0.1f;

		upDirection = Vec3(0.0f, 1.0f, 0.0f);
		position = Vec3(0.0f, 0.0f, 0.0f);
		slopeLimit = 0.1f;
		contactOffset = 0.1f;
		stepOffset = 0.1f;
		scaleCoeff = 0.9f;
		volumeGrowth = 1.5f;
		invisibleWallHeight = 0.0f;
		maxJumpHeight = 0.0f;
	}

	/**
	*/
	PhysXCharacterController::PhysXCharacterController(CharacterControllerDesc&_desc)
		:desc(_desc),
		mController(nullptr)
	{
		if (desc.mMode == desc.CCT_CAPSULE)
			_createCapsuleCharacterController();
		else
			_createBoxCharacterController();
	}

	/**
	*/
	PhysXCharacterController::PhysXCharacterController(const CharacterControllerDesc & _desc)
		:desc(_desc),
		mController(nullptr)
	{
		if (desc.mMode == desc.CCT_CAPSULE)
			_createCapsuleCharacterController();
		else
			_createBoxCharacterController();
	}

	/**
	*/
	PhysXCharacterController::~PhysXCharacterController()
	{
		mController->release();
	}

	/**
	*/
	void PhysXCharacterController::_createCapsuleCharacterController()
	{
		PxCapsuleControllerDesc pdesc;
		pdesc.height = desc.height;

		pdesc.radius = desc.radius;
		pdesc.material = GetPhysics()->GetPxMaterial();
		pdesc.density = desc.density;

		pdesc.contactOffset = desc.contactOffset;

		pdesc.stepOffset = desc.stepOffset;

		pdesc.invisibleWallHeight = desc.invisibleWallHeight;
		pdesc.maxJumpHeight = desc.maxJumpHeight;
		pdesc.scaleCoeff = desc.scaleCoeff;
		pdesc.climbingMode = PxCapsuleClimbingMode::eEASY;
		pdesc.slopeLimit = desc.slopeLimit;
		pdesc.volumeGrowth = desc.volumeGrowth;

		pdesc.position = PxExtendedVec3(desc.position.x, desc.position.y, desc.position.z);
		pdesc.upDirection = PxVec3(desc.upDirection.x, desc.upDirection.y, desc.upDirection.z);

		pdesc.material = GetPhysics()->GetPxMaterial();
		mController = GetPhysics()->GetPxControllerManager()->createController(pdesc);
	}

	/**
	*/
	void PhysXCharacterController::_createBoxCharacterController()
	{
		PxBoxControllerDesc pdesc;
		pdesc.halfForwardExtent = desc.radius;
		pdesc.halfSideExtent = desc.radius;
		pdesc.halfHeight = desc.height / 2;


		pdesc.density = desc.density;
		pdesc.material = GetPhysics()->GetPxMaterial();

		pdesc.contactOffset = desc.contactOffset;

		pdesc.stepOffset = desc.stepOffset;

		pdesc.invisibleWallHeight = desc.invisibleWallHeight;
		pdesc.maxJumpHeight = desc.maxJumpHeight;
		pdesc.scaleCoeff = desc.scaleCoeff;
		pdesc.slopeLimit = desc.slopeLimit;
		pdesc.volumeGrowth = desc.volumeGrowth;

		pdesc.position = PxExtendedVec3(desc.position.x, desc.position.y, desc.position.z);
		pdesc.upDirection = PxVec3(desc.upDirection.x, desc.upDirection.y, desc.upDirection.z);

		pdesc.material = GetPhysics()->GetPxMaterial();
		mController = GetPhysics()->GetPxControllerManager()->createController(pdesc);
	}

	/**
	*/
	void PhysXCharacterController::move(float x, float y, float z, float elapsedTime)
	{
		PxVec3 moveVec(x, y, z);
		static PxControllerFilters filters(0);
		if (mController)
		{
			GetPhysics()->LockWrite();
			mController->move(moveVec, 0.01f, elapsedTime, filters);
			fpos = mController->getPosition();
			GetPhysics()->UnLockWrite();
		}
	}

	/**
	*/
	Vec3 PhysXCharacterController::getPosition()
	{
		mPos = Vec3(fpos.x, fpos.y, fpos.z);
		return mPos;
	}

	/**
	*/
	void PhysXCharacterController::Jump(Vec3 &movement)
	{
		if (CanJump())
		{
			mJump.StartJump(desc.maxJumpHeight);

			float dtime = GetPhysics()->GetDTime();
			float dy;
			const float heightDelta = _JumpGetHeight(dtime, desc.maxJumpHeight);
			if (heightDelta != 0.0f)
			{
				dy = heightDelta;
				movement.y += dy;
			}


			mJump.StopJump();
		}
		else Debug("can't jump");
	}

	/**
	*/
	bool PhysXCharacterController::CanJump()
	{
		PxControllerState cctState;
		mController->getState(cctState);
		return (cctState.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN) != 0;
	}

	/**
	*/
	float PhysXCharacterController::_JumpGetHeight(float _f, float _descHeight)
	{
		return mJump.GetHeight(_f, _descHeight);
	}
}