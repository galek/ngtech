/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Light.h"
#include "Frustum.h"
#include "Config.h"
#include "WindowSystem.h"
#include "GLSystem.h"
#include "Scene.h"
#include "../../Core/inc/CVARManager.h"
//**************************************

namespace NGTech {
	Light::Light(){
		GetScene()->addLight(this);
	}

	//**************************************************************************
	//LightOmni
	//**************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new omni light
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	LightOmni::LightOmni() {
		this->color = Vec3(1, 1, 1);
		this->position = Vec3(0, 0, 0);
		this->radius = 200;

		this->castShadows = true;

		int size = GetCvars()->r_shadowsize;
		shadowMap = GetRender()->TextureCreateCube(size, size, GLTexture::RGBA);
		shadowMap->setFilter(GLTexture::LINEAR);
		shadowMap->setAniso(GLTexture::ANISO_X0);
		shadowMap->setWrap(GLTexture::CLAMP_TO_EDGE);
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys omni light
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	LightOmni::~LightOmni() {
		delete shadowMap;
	}

#include "Log.h"
	//---------------------------------------------------------------------------
	//Desc:    gets lights scissor rectangle
	//Params:  cameraPos - camera position, x, y, z, w - output rect coords
	//Returns: -
	//---------------------------------------------------------------------------
	void LightOmni::getScissorRect(const Vec3 &cameraPos, int &x, int &y, int &z, int &w) {
		int viewport[4];
		GetRender()->getViewport(viewport);

		if ((position - cameraPos).length() < radius*1.5) {
			x = viewport[0];
			y = viewport[1];
			z = viewport[2];
			w = viewport[3];
			return;
		}

		Mat4 tmodelview = Mat4::transpose(GetRender()->getMatrix_Modelview());
		Mat4 mvp = GetRender()->getMatrix_MVP();

		Vec3 vx = tmodelview * Vec3(radius, 0, 0);
		Vec3 vy = tmodelview * Vec3(0, radius, 0);

		Vec4 p[4];
		p[0] = mvp * Vec4(position - vx, 1);
		p[1] = mvp * Vec4(position + vx, 1);
		p[2] = mvp * Vec4(position - vy, 1);
		p[3] = mvp * Vec4(position + vy, 1);

		p[0] = p[0] / p[0].w;
		p[1] = p[1] / p[1].w;
		p[2] = p[2] / p[2].w;
		p[3] = p[3] / p[3].w;

		if (p[0].x < p[2].x) {
			x = viewport[0] + (int) ((float) viewport[2] * (p[0].x + 1.0) / 2.0);
			z = viewport[0] + (int) ((float) viewport[2] * (p[1].x + 1.0) / 2.0);
		}
		else {
			x = viewport[0] + (int) ((float) viewport[2] * (p[1].x + 1.0) / 2.0);
			z = viewport[0] + (int) ((float) viewport[2] * (p[0].x + 1.0) / 2.0);
		}

		if (p[1].y < p[3].y) {
			y = viewport[1] + (int) ((float) viewport[3] * (p[2].y + 1.0) / 2.0);
			w = viewport[1] + (int) ((float) viewport[3] * (p[3].y + 1.0) / 2.0);
		}
		else {
			y = viewport[1] + (int) ((float) viewport[3] * (p[3].y + 1.0) / 2.0);
			w = viewport[1] + (int) ((float) viewport[3] * (p[2].y + 1.0) / 2.0);
		}

		if (x < viewport[0])
			x = viewport[0];
		else if (x > viewport[0] + viewport[2])
			x = viewport[0] + viewport[2];

		if (y < viewport[1])
			y = viewport[1];
		else if (y > viewport[1] + viewport[3])
			y = viewport[1] + viewport[3];

		if (z < viewport[0])
			z = viewport[0];
		else if (z > viewport[2] + viewport[3])
			z = viewport[0] + viewport[2];

		if (w < viewport[1])
			w = viewport[1];
		else if (w > viewport[1] + viewport[3])
			w = viewport[1] + viewport[3];

		z -= x;
		w -= y;
	}

	//**************************************************************************
	//LightSpot
	//**************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new direct light
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	LightSpot::LightSpot() {
		this->color = Vec3(1, 1, 1);
		this->position = Vec3(0, 0, 0);
		this->direction = Vec3(1, 0, 0);
		this->radius = 200;
		this->fov = 60;

		this->castShadows = true;

		int size = GetCvars()->r_shadowsize;

		shadowMap = GetRender()->TextureCreate2D(size, size, GLTexture::RGBA);
		shadowMap->setFilter(GLTexture::LINEAR);
		shadowMap->setWrap(GLTexture::CLAMP);

		projMap = GetRender()->TextureCreate2D("../data/textures/effects/spot.jpg");
		projMap->setWrap(GLTexture::CLAMP);
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys direct light
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	LightSpot::~LightSpot() {
		delete shadowMap;
		delete projMap;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets lights scissor rectangle
	//Params:  cameraPos - camera position, x, y, z, w - output rect coords
	//Returns: -
	//---------------------------------------------------------------------------
	void LightSpot::getScissorRect(const Vec3 &cameraPos, int &x, int &y, int &z, int &w) {
		int viewport[4];
		GetRender()->getViewport(viewport);

		if ((position - cameraPos).length() < radius*1.5) {
			x = viewport[0];
			y = viewport[1];
			z = viewport[2];
			w = viewport[3];
			return;
		}

		Mat4 tmodelview = Mat4::transpose(GetRender()->getMatrix_Modelview());
		Mat4 mvp = GetRender()->getMatrix_MVP();

		Vec3 vx = tmodelview * Vec3(radius, 0, 0);
		Vec3 vy = tmodelview * Vec3(0, radius, 0);

		Vec4 p[4];
		p[0] = mvp * Vec4(position - vx, 1);
		p[1] = mvp * Vec4(position + vx, 1);
		p[2] = mvp * Vec4(position - vy, 1);
		p[3] = mvp * Vec4(position + vy, 1);

		p[0] = p[0] / p[0].w;
		p[1] = p[1] / p[1].w;
		p[2] = p[2] / p[2].w;
		p[3] = p[3] / p[3].w;

		if (p[0].x < p[2].x) {
			x = viewport[0] + (int) ((float) viewport[2] * (p[0].x + 1.0) / 2.0);
			z = viewport[0] + (int) ((float) viewport[2] * (p[1].x + 1.0) / 2.0);
		}
		else {
			x = viewport[0] + (int) ((float) viewport[2] * (p[1].x + 1.0) / 2.0);
			z = viewport[0] + (int) ((float) viewport[2] * (p[0].x + 1.0) / 2.0);
		}

		if (p[1].y < p[3].y) {
			y = viewport[1] + (int) ((float) viewport[3] * (p[2].y + 1.0) / 2.0);
			w = viewport[1] + (int) ((float) viewport[3] * (p[3].y + 1.0) / 2.0);
		}
		else {
			y = viewport[1] + (int) ((float) viewport[3] * (p[3].y + 1.0) / 2.0);
			w = viewport[1] + (int) ((float) viewport[3] * (p[2].y + 1.0) / 2.0);
		}

		if (x < viewport[0])
			x = viewport[0];
		else if (x > viewport[0] + viewport[2])
			x = viewport[0] + viewport[2];

		if (y < viewport[1])
			y = viewport[1];
		else if (y > viewport[1] + viewport[3])
			y = viewport[1] + viewport[3];

		if (z < viewport[0])
			z = viewport[0];
		else if (z > viewport[2] + viewport[3])
			z = viewport[0] + viewport[2];

		if (w < viewport[1])
			w = viewport[1];
		else if (w > viewport[1] + viewport[3])
			w = viewport[1] + viewport[3];

		z -= x;
		w -= y;
	}

	//**************************************************************************
	//LightDirect
	//**************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new direct light
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	LightDirect::LightDirect() {
		this->color = Vec3(1, 1, 1);
		this->direction = Vec3(1, 1, 1);
	}

	//---------------------------------------------------------------------------
	//Desc:    destroys direct light
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	LightDirect::~LightDirect() {
	}

}
