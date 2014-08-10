#pragma once

#include "../../Core/inc/MathLib.h"

using namespace System;
using namespace NGTech;

namespace EngineCLR {

	public ref class Vec3
	{
	public:
		Vec3() :vec(nullptr)
		{
			vec = new NGTech::Vec3();
		}

		Vec3(float x, float y, float z) :Vec3()
		{
			vec->x = x;
			vec->y = y;
			vec->z = z;
		}

		~Vec3()
		{
			delete vec;
		}
	private:
		NGTech::Vec3*vec;
	};
}