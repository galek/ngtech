/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
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

namespace NGTech {

	/**
	*/
	CameraFPS::CameraFPS() {
		this->position = Vec3(0, 0, 0);
		this->maxVelocity = 1500;
		this->fov = 60;

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
	void CameraFPS::setPhysics(const Vec3 &size, float mass) {
		/*pBody = PhysBody::CreateCapsule(size.x, size.y, &Mat4::translate(position), mass);
		this->size = size;
		pJoint = new PhysJointUpVector(Vec3(0, 1, 0), pBody);*/
	}

	/**
	*/
	Mat4 CameraFPS::getTransform() {
		return Mat4::lookAt(position, position + direction, Vec3(0, 1, 0));
	}

	/**
	*/
	Mat4 CameraFPS::getProjection() {
		return Mat4::perspective(fov, (float)4 / (float)3, 0.1, 10000);
	}

	/**
	*/
	void CameraFPS::update() {
		/*if(pBody) {
		position = pBody->GetTransform().getTranslation() + Vec3(0, 7, 0);
		pBody->SetLinearVelocity(Vec3(0, pBody->GetLinearVelocity().y, 0));
		}*/

		if (GetWindow()->isMouseMoved() && GetWindow()->isMouseGrabed()) {
			angle[0] = -0.4 * GetWindow()->getMouseX();
			angle[1] = -0.4 * GetWindow()->getMouseY();
		}

		if (angle[1] > 80) angle[1] = 75;
		if (angle[1] < -80) angle[1] = -75;

		Vec3 forwardVec = Vec3(sinf(DEG_TO_RAD * angle[0]), 0, cosf(DEG_TO_RAD * angle[0]));
		Vec3 leftVec = Vec3(sinf(DEG_TO_RAD * (angle[0] + 90)), 0, cosf(DEG_TO_RAD * (angle[0] + 90)));
		Vec3 movement = Vec3(0, 0, 0);

		direction.x = sinf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction.y = sinf(DEG_TO_RAD * angle[1]);
		direction.z = cosf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction = Vec3::normalize(direction);

		bool inTheAir = false;
		Vec3 point;
		if (GetPhysics()->intersectWorldByRay(position - Vec3(0, size.y, 0),
			position - Vec3(0, size.y + 10, 0), Vec3(),
			point)) {
			if (point.y <= position.y - size.y - 10)
				inTheAir = true;
		}

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

		if (inTheAir) {
			movement = movement * 0.5;
		}

		if (movement.length() > EPSILON) {
			movement = Vec3::normalize(movement);
		}

		if (GetWindow()->isKeyDown("q") && !inTheAir) {
			movement += Vec3(0, 1.5, 0);
		}
		transform = Mat4::translate(position) * Mat4::rotate(90, direction);
		view = Mat4::lookAt(position, position + direction, Vec3(0, 1, 0));
	}
}