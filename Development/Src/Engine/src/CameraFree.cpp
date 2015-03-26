/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    CameraFree.cpp
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

namespace NGTech {

	/**
	*/
	CameraFree::CameraFree() :Camera() {
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
	CameraFree::~CameraFree() {
		SAFE_DELETE(pBody);
	}

	/**
	*/
	void CameraFree::setPhysics(const Vec3 &size, float mass) {
		CharacterControllerDesc desc;
		desc.height = 10.f;
		desc.contactOffset = 0.05f; // Originally had it lower, raising it didn't seem to do much.
		desc.position = position;
		desc.radius = 5 * 0.4;
		desc.upDirection = Vec3(0, 1, 0);
		desc.stepOffset = 0.01f;
		pBody = new PhysXCharacterController(desc);
	}

	/**
	*/
	void CameraFree::update() {
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

		if (pBody)
			pBody->move(movement.x, movement.y, movement.z, GetEngine()->GetTimePerFrame());
		else
			position += movement;

		transform = Mat4::translate(position) * Mat4::rotate(90, direction);
		view = Mat4::lookAt(position, position + direction, Vec3(0, 1, 0));
	}

}