/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    Frustum.cpp
* Desc:    Frustum impl.
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
#include <math.h>
#include "Frustum.h"
#include "GLSystem.h"
//**************************************

namespace NGTech {

/*
*/
bool Frustum::isInside(const Vec3 &point) {
		for(int p = 0; p < 6; p++) {
			if(plane[p][0] * point.x + plane[p][1] * point.y + plane[p][2] * point.z + plane[p][3] < 0)
				return false;
		}
		return true;
	}

/*
*/
bool Frustum::isInside(const Vec3 &center, float radius) {
		for(int p = 0; p < 6; p++ ) {
			if(plane[p][0] * center.x + plane[p][1] * center.y + plane[p][2] * center.z + plane[p][3] < -radius)
				return false;
		}
		return true;
	}

/*
*/
bool Frustum::isInside(const Vec3 &min, const Vec3 &max) {
		for(int p = 0; p < 6; p++)
		{
			if(plane[p][0] * (min[0]) + plane[p][1] * (min[1]) + plane[p][2] * (min[2]) + plane[p][3] > 0)
				continue;
			if(plane[p][0] * (max[0]) + plane[p][1] * (min[1]) + plane[p][2] * (min[2]) + plane[p][3] > 0)
				continue;
			if(plane[p][0] * (min[0]) + plane[p][1] * (max[1]) + plane[p][2] * (min[2]) + plane[p][3] > 0)
				continue;
			if(plane[p][0] * (max[0]) + plane[p][1] * (max[1]) + plane[p][2] * (min[2]) + plane[p][3] > 0)
				continue;
			if(plane[p][0] * (min[0]) + plane[p][1] * (min[1]) + plane[p][2] * (max[2]) + plane[p][3] > 0)
				continue;
			if(plane[p][0] * (max[0]) + plane[p][1] * (min[1]) + plane[p][2] * (max[2]) + plane[p][3] > 0)
				continue;
			if(plane[p][0] * (min[0]) + plane[p][1] * (max[1]) + plane[p][2] * (max[2]) + plane[p][3] > 0)
				continue;
			if(plane[p][0] * (max[0]) + plane[p][1] * (max[1]) + plane[p][2] * (max[2]) + plane[p][3] > 0)
				continue;
			return false;
		}
		return true;
	}

/*
*/
void Frustum::get() {
		Mat4 modl, proj;

		proj = GetRender()->getMatrix_Projection();
		modl = GetRender()->getMatrix_Modelview();

		Mat4 clip = proj * modl;

		float d;

		plane[0][0] = clip.e[ 3] - clip.e[ 0];
		plane[0][1] = clip.e[ 7] - clip.e[ 4];
		plane[0][2] = clip.e[11] - clip.e[ 8];
		plane[0][3] = clip.e[15] - clip.e[12];
		d = sqrt(plane[0][0]*plane[0][0] + plane[0][1]*plane[0][1] + plane[0][2]*plane[0][2]);
		plane[0][0] *= 1/d;
		plane[0][1] *= 1/d;
		plane[0][2] *= 1/d;
		plane[0][3] *= 1/d;

		plane[1][0] = clip.e[ 3] + clip.e[ 0];
		plane[1][1] = clip.e[ 7] + clip.e[ 4];
		plane[1][2] = clip.e[11] + clip.e[ 8];
		plane[1][3] = clip.e[15] + clip.e[12];
		d = sqrt(plane[1][0]*plane[1][0] + plane[1][1]*plane[1][1] + plane[1][2]*plane[1][2]);
		plane[1][0] *= 1/d;
		plane[1][1] *= 1/d;
		plane[1][2] *= 1/d;
		plane[1][3] *= 1/d;

		plane[2][0] = clip.e[ 3] + clip.e[ 1];
		plane[2][1] = clip.e[ 7] + clip.e[ 5];
		plane[2][2] = clip.e[11] + clip.e[ 9];
		plane[2][3] = clip.e[15] + clip.e[13];
		d = sqrt(plane[2][0]*plane[2][0] + plane[2][1]*plane[2][1] + plane[2][2]*plane[2][2]);
		plane[2][0] *= 1/d;
		plane[2][1] *= 1/d;
		plane[2][2] *= 1/d;
		plane[2][3] *= 1/d;

		plane[3][0] = clip.e[ 3] - clip.e[ 1];
		plane[3][1] = clip.e[ 7] - clip.e[ 5];
		plane[3][2] = clip.e[11] - clip.e[ 9];
		plane[3][3] = clip.e[15] - clip.e[13];
		d = sqrt(plane[3][0]*plane[3][0] + plane[3][1]*plane[3][1] + plane[3][2]*plane[3][2]);
		plane[3][0] *= 1/d;
		plane[3][1] *= 1/d;
		plane[3][2] *= 1/d;
		plane[3][3] *= 1/d;

		plane[4][0] = clip.e[ 3] - clip.e[ 2];
		plane[4][1] = clip.e[ 7] - clip.e[ 6];
		plane[4][2] = clip.e[11] - clip.e[10];
		plane[4][3] = clip.e[15] - clip.e[14];
		d = sqrt(plane[4][0]*plane[4][0] + plane[4][1]*plane[4][1] + plane[4][2]*plane[4][2]);
		plane[4][0] *= 1/d;
		plane[4][1] *= 1/d;
		plane[4][2] *= 1/d;
		plane[4][3] *= 1/d;

		plane[5][0] = clip.e[ 3] + clip.e[ 2];
		plane[5][1] = clip.e[ 7] + clip.e[ 6];
		plane[5][2] = clip.e[11] + clip.e[10];
		plane[5][3] = clip.e[15] + clip.e[14];
		d = sqrt(plane[5][0]*plane[5][0] + plane[5][1]*plane[5][1] + plane[5][2]*plane[5][2]);
		plane[5][0] *= 1/d;
		plane[5][1] *= 1/d;
		plane[5][2] *= 1/d;
		plane[5][3] *= 1/d;
	}

}