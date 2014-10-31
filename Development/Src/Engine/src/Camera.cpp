/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    Camera.cpp
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

//TODO:Фиксированную камеру

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
	Camera::Camera() {
		if (GetScene())
			GetScene()->setCamera(this);
	}


	/**
	*/
	Mat4 Camera::getTransform() {
		return Mat4::lookAt(position, position + direction, Vec3(0, 1, 0));
	}

	/**
	*/
	Mat4 Camera::getProjection() {
		return Mat4::perspective(fov, (float)4 / (float)3, 0.1, 10000);
	}
}