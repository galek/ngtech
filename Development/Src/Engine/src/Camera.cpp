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
		view.Identity();
		projection.Identity();

		if (GetScene())
			GetScene()->setCamera(this);

		this->position = Vec3(0.0f, 0.0f, 0.0f);
		this->direction = Vec3(0.0f, 0.0f, 1.0f);
		this->upVector = Vec3(0.0f, 1.0f, 0.0f);
		//this->RecalculateView();


		this->fov = 60.0f;
		this->aspect = 4.0f / 3.0f;
		this->zNear = 0.1f;
		this->zFar = 1e4f;
		//this->RecalculateProjection();

		this->frustum = new Frustum();
		this->RecalculateFrustum();

	}


	/**
	*/
	Mat4 Camera::getTransform() {
		return Mat4::lookAt(getPosition(), getPosition() + direction, Vec3(0, 1, 0));
	}

	/**
	*/
	void Camera::RecalculateProjection() {
		projection = Mat4::perspective(fov, aspect, zNear, zFar);
	}

	/**
	*/
	void Camera::RecalculateFrustum()
	{
		frustum->Build(projection * view);
	}

	/*
	*/
	void Camera::RecalculateView()
	{
		view = Mat4::lookAt(position, direction, upVector);
	}

	/**
	*/
	void Camera::SetView(const Mat4 &view)
	{
		this->view = view;
		position = Mat4::inverse(view) * Vec3(0.0f, 0.0f, 0.0f);
	}
}