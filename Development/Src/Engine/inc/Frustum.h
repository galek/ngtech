/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//*************************
#include "../../Core/inc/MathLib.h"
#include "../../Core/inc/BBox.h"
#include "../../Core/inc/BSphere.h"
//*************************

namespace NGTech {

	/**
	View frustum
	*/
	class Frustum {
	public:

		/**
		gets the current view frustum from Meshview and Projection matrix
		*/
		void Get();

		/**
		Gets the current view frustum from modelview and projection matrix
		*/
		void Build(const Mat4 &matrix);

		/**
		checks wether the point is inside of the frustum
		\param point  point coordinates
		\return true if inside
		*/
		bool isInside(const Vec3 &point);

		/**
		checks weather the bounding box is inside the frustum
		\param min box`s min and max
		\param max  sphere radius
		\return true if inside
		*/
		bool isInside(const Vec3 &min, const Vec3 &max);

		/**
		checks weather the bounding sphere is inside the frustum
		\param center  sphere center
		\param radius  sphere radius
		\return true if inside
		*/
		bool isInside(const Vec3 &center, float radius);

		/**
		checks weather the bounding box is inside the frustum
		\param min box`s min and max
		\param max  sphere radius
		\return true if  inside
		*/
		bool isInside(const BBox &box);

		/**
		checks weather the bounding sphere is inside the frustum
		\param center  sphere center
		\param radius  sphere radius
		\return true if  inside
		*/
		bool isInside(const BSphere &sphere);
	private:
		float plane[6][4];
	};
}