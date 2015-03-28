/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    CameraFPS.cpp
* Desc:    Cameras impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
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

#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Camera.h"
#include "WindowSystem.h"
#include "Log.h"
#include "Scene.h"
//**************************************
#include "PhysXCharacterController.h"
//**************************************
//see http://physx3.googlecode.com/svn/trunk/PhysX-3.2_PC_SDK_Core/Samples/SampleCCTSharedCode/SampleCCTActor.cpp
namespace NGTech {
	/**
	*/
	CameraFPS::CameraFPS() {
		this->position = Vec3(0, 0, 0);
		this->maxVelocity = 1500;
		this->fov = 60;
		this->aspect = 4.0 / 3.0;
		this->zNear = 0.1;
		this->zFar = 1e4;
		pBody = NULL;
	}

	/**
	*/
	CameraFPS::~CameraFPS() {
		if (pBody) {
			delete pBody;
			delete pJoint;
		}
	}

	/**
	*/
	void CameraFPS::setPhysics(const Vec3 &_size) {
		size = _size;
		CharacterControllerDesc desc;
		desc.height = _size.y;
		desc.contactOffset = 0.05f; // Originally had it lower, raising it didn't seem to do much.
		desc.position = position;
		desc.radius = 5 * 0.4;
		desc.upDirection = Vec3(0, 1, 0);
		desc.stepOffset = 0.01f;
		desc.maxJumpHeight = 2.46f;//–екорд мира,в прыжке,в высоту

		desc.slopeLimit = 45.0f;//”гол наклона
		desc.contactOffset = 0.1f;
		desc.stepOffset = 0.1f;
		desc.invisibleWallHeight = 0.0f;

		desc.scaleCoeff = 1.f;//Nick:TODO:сделать возможность задавать Scale дл€ камеры

		pBody = new PhysXCharacterController(desc);
	}

	/**
	*/
	void CameraFPS::setPhysics(const CharacterControllerDesc * desc)
	{
		pBody = new PhysXCharacterController(*desc);
	}

	/**
	*/
	void CameraFPS::update() {

		if (pBody)
			position = pBody->getPosition();

		if (GetWindow()->isMouseMoved() && GetWindow()->isMouseGrabed()) {
			angle[0] = -0.4 * GetWindow()->getMouseX();
			angle[1] = -0.4 * GetWindow()->getMouseY();
		}

		if (angle[1] > 80) angle[1] = 75;
		if (angle[1] < -80) angle[1] = -75;

		Vec3 forwardVec = Vec3(sinf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]),
			sinf(DEG_TO_RAD * angle[1]),
			cosf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]));

		Vec3 leftVec = Vec3(sinf(DEG_TO_RAD * (angle[0] + 90)), 0, cosf(DEG_TO_RAD * (angle[0] + 90)));
		Vec3 movement = Vec3(0, 0, 0);


		direction.x = sinf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction.y = sinf(DEG_TO_RAD * angle[1]);
		direction.z = cosf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction = Vec3::normalize(direction);

		Vec3 point;
		float dtime = GetPhysics()->GetDTime();

		if (GetWindow()->isKeyPressed("W")) {
			movement += forwardVec;
		}
		if (GetWindow()->isKeyPressed("S")) {
			movement -= forwardVec;
		}
		if (GetWindow()->isKeyPressed("A")) {
			movement += leftVec;
		}
		if (GetWindow()->isKeyPressed("D")) {
			movement -= leftVec;
		}

		if (movement.length() > EPSILON) {
			movement = Vec3::normalize(movement);
		}

		static bool jump = false;
		if (GetWindow()->isKeyDown("SPACE")) {
			jump = true;
			pBody->Jump(movement);
		}
		else {
			if (!jump)
			{
				float dy = -9.81f * dtime;
				movement.y += dy;
			}
			else
				jump = false;
		}

		if (pBody)
		{
			movement *= dtime;
			pBody->move(movement.x, movement.y, movement.z, GetEngine()->GetTimePerFrame());
		}
		else
			position += movement;
		transform = Mat4::translate(position) * Mat4::rotate(90, direction);
		view = Mat4::lookAt(position, position + direction, Vec3(0, 1, 0));
	}
}