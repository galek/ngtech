/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    PhysXCharacterController.h
* Desc:    PhysX Character Controller
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
#pragma once

//***************************************************************************
#include "PxPhysicsAPI.h"
//***************************************************************************

namespace physx
{
	class PxController;
	struct PxExtendedVec3;
}

namespace NGTech
{

	struct CharacterControllerDesc
	{
		enum Mode
		{
			CCT_CAPSULE,
			CCT_BOX
		};

		CharacterControllerDesc(Mode = CCT_CAPSULE);

		float height;
		float radius;
		float density;
		float contactOffset;
		float stepOffset;
		float invisibleWallHeight;
		float maxJumpHeight;
		float scaleCoeff;
		float slopeLimit;
		float volumeGrowth;
		Vec3 position;
		Vec3 upDirection;
		Mode mMode;
	};


	class PhysXCharacterController
	{
	public:
		PhysXCharacterController(CharacterControllerDesc&desc);
		~PhysXCharacterController();

		void move(float x, float y, float z, float elapsedTime);
		Vec3 getPosition();
		void Jump(Vec3 &height);
		bool CanJump();
	private:
		class JumpAction
		{
		public:
			JumpAction();

			float		mV0;
			float		mJumpTime;
			bool		mJump;

			void		StartJump(float v0);
			void		StopJump();
			float		GetHeight(float elapsedTime,float _descHeight);
		};
		JumpAction mJump;
	private:
		float _JumpGetHeight(float _time,float _descHeight);
		void _createCapsuleCharacterController();
		void _createBoxCharacterController();
	private:
		physx::PxController* mController;
		physx::PxExtendedVec3 fpos;
		CharacterControllerDesc desc;
		Vec3 mPos;
	};
}