/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//*************************
#include "MathLib.h"
//*************************

namespace VEGA {

//---------------------------------------------------------------------------
//Desc: View frustum struct
//---------------------------------------------------------------------------
class ViewFrustum {
public:
	void get();
	double *getPlane(int n) { return plane[n]; };

	bool isInside(const Vec3 &point);
	bool isInside(const Vec3 &min, const Vec3 &max);
	bool isInside(const Vec3 &center, float radius);

private:
	double plane[6][4];
};

};

