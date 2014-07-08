/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    Camera.cpp
* Desc:    Cameras impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/

#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Camera.h"
#include "WindowSystem.h"
#include "Log.h"
#include "Scene.h"
//**************************************

namespace VEGA {

	//***************************************************************************
	//FPS Camera class
	//***************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new FPS camera
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	CameraFPS::CameraFPS() {
		this->position = Vec3(0, 0, 0);
		this->maxVelocity = 1500;
		this->fov = 60;

		pBody = NULL;
	}

	//---------------------------------------------------------------------------
	//Desc:    Camera destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	CameraFPS::~CameraFPS() {
		if(pBody) { 
			delete pBody; 
			delete pJoint;
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    set physics to camera
	//Params:  size - bounding sphere size, mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void CameraFPS::setPhysics(const Vec3 &size, float mass) {
		pBody = PhysBody::createCapsule(size.x,size.y, mass);
		pBody->setTransform(Mat4::translate(position));
		this->size = size;
		pJoint = new PhysJointUpVector(Vec3(0, 1, 0), pBody);
	}

	//---------------------------------------------------------------------------
	//Desc:    get camera modelview matrix
	//Params:  -
	//Returns: modelview matrix
	//---------------------------------------------------------------------------
	Mat4 CameraFPS::getTransform() {
		return Mat4::lookAt(position, position + direction, Vec3(0, 1, 0));
	}

	//---------------------------------------------------------------------------
	//Desc:    get camera projection matrix
	//Params:  -
	//Returns: projection matrix
	//---------------------------------------------------------------------------
	Mat4 CameraFPS::getProjection() {
		return Mat4::perspective(fov, (float)4/(float)3, 0.1, 10000);
	}

	//---------------------------------------------------------------------------
	//Desc:    update camera
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void CameraFPS::update() {
		if(pBody) {
			position = pBody->getTransform().getTranslation() + Vec3(0, 7, 0);
			pBody->setVelocity(Vec3(0, pBody->getVelocity().y, 0));//pBody->getVelocity().y, 0));
		}

		/*Vec3 forwardVec = Vec3::normalize(Vec3(sinf(angle[0]) * cosf(angle[1]),
		sinf(angle[1]),
		cosf(angle[0]) * cosf(angle[1])));*/
		if(GetEngine()->iWindow->isMouseMoved() && GetEngine()->iWindow->isMouseGrabed()) {
			angle[0] = -0.4 * GetEngine()->iWindow->getMouseX();
			angle[1] = -0.4 * GetEngine()->iWindow->getMouseY();
		}

		if(angle[1] > 80) angle[1] = 75;
		if(angle[1] < -80) angle[1] = -75;

		Vec3 forwardVec = Vec3(sinf(DEG_TO_RAD * angle[0]), 0, cosf(DEG_TO_RAD * angle[0]));
		Vec3 leftVec    = Vec3(sinf(DEG_TO_RAD * (angle[0] + 90)), 0, cosf(DEG_TO_RAD * (angle[0] + 90)));
		Vec3 movement = Vec3(0, 0, 0);

		direction.x = sinf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction.y = sinf(DEG_TO_RAD * angle[1]);
		direction.z = cosf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction = Vec3::normalize(direction);

		bool inTheAir = false;
		Vec3 point;
		if(GetEngine()->physSystem->intersectWorldByRay(position - Vec3(0, size.y, 0), 
			position - Vec3(0, size.y+10, 0), Vec3(), 
			point)) {
				if(point.y <= position.y - size.y - 10)
					inTheAir = true;
		}

		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_W)) {
			movement += forwardVec;
		}
		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_S)) {
			movement -= forwardVec;
		}
		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_A)) {
			movement += leftVec;
		}
		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_D)) {
			movement -= leftVec;
		}

		if(inTheAir) {
			movement = movement * 0.5;
		}

		if(movement.length() > EPSILON) {
			movement = Vec3::normalize(movement);
		}

		if(GetEngine()->iWindow->isKeyDown(WindowSystem::KEY_Q) && !inTheAir) {
			movement += Vec3(0, 1.5, 0);
		}

		pBody->addTorque(movement * maxVelocity);
	}


	Camera::Camera() {
		if(GetEngine()->scene)
			GetEngine()->scene->setCamera(this);
	}

	//***************************************************************************
	//CameraFree class
	//***************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new free camera
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	CameraFree::CameraFree():Camera() {
		this->position = Vec3(0, 0, 0);
		this->maxVelocity = 1500;
		this->fov = 60;

		pBody = NULL;
		setPhysics(Vec3(5, 5, 5), 0.001);
	}

	//---------------------------------------------------------------------------
	//Desc:    Camera destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	CameraFree::~CameraFree() {
		if(pBody) { delete pBody; }
	}

	//---------------------------------------------------------------------------
	//Desc:    set physics to camera
	//Params:  size - bounding sphere size, mass - object mass
	//Returns: -
	//---------------------------------------------------------------------------
	void CameraFree::setPhysics(const Vec3 &size, float mass) {
		mass = 0.001;
		pBody = PhysBody::createSphere(size.y, mass);
		pBody->setTransform(Mat4::translate(position));
	}

	//---------------------------------------------------------------------------
	//Desc:    get camera modelview matrix
	//Params:  -
	//Returns: modelview matrix
	//---------------------------------------------------------------------------
	Mat4 CameraFree::getTransform() {
		return Mat4::lookAt(position, position + direction, Vec3(0, 1, 0));
	}

	//---------------------------------------------------------------------------
	//Desc:    get camera projection matrix
	//Params:  -
	//Returns: projection matrix
	//---------------------------------------------------------------------------
	Mat4 CameraFree::getProjection() {
		return Mat4::perspective(fov, (float)4/(float)3, 0.1, 10000);
	}

	//---------------------------------------------------------------------------
	//Desc:    update camera
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void CameraFree::update() {
		if(pBody) {
			position = pBody->getTransform().getTranslation();
			pBody->setVelocity(Vec3(0, 0, 0));
		}

		/*Vec3 forwardVec = Vec3::normalize(Vec3(sinf(angle[0]) * cosf(angle[1]),
		sinf(angle[1]),
		cosf(angle[0]) * cosf(angle[1])));*/
		if(GetEngine()->iWindow->isMouseMoved() && GetEngine()->iWindow->isMouseGrabed()) {
			angle[0] = -0.4 * GetEngine()->iWindow->getMouseX();
			angle[1] = -0.4 * GetEngine()->iWindow->getMouseY();
		}

		if(angle[1] > 80) angle[1] = 75;
		if(angle[1] < -80) angle[1] = -75;

		Vec3 forwardVec = Vec3(sinf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]), 
			sinf(DEG_TO_RAD * angle[1]), 
			cosf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]));

		Vec3 leftVec    = Vec3(sinf(DEG_TO_RAD * (angle[0] + 90)), 0, cosf(DEG_TO_RAD * (angle[0] + 90)));
		Vec3 movement = Vec3(0, 0, 0);

		direction.x = sinf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction.y = sinf(DEG_TO_RAD * angle[1]);
		direction.z = cosf(DEG_TO_RAD * angle[0]) * cosf(DEG_TO_RAD * angle[1]);
		direction = Vec3::normalize(direction);

		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_W)) {
			movement += forwardVec;
		}
		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_S)) {
			movement -= forwardVec;
		}
		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_A)) {
			movement += leftVec;
		}
		if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_D)) {
			movement -= leftVec;
		}

		if(movement.length() > EPSILON) {
			movement = Vec3::normalize(movement);
		}

		pBody->addTorque(movement * maxVelocity);
	}

}