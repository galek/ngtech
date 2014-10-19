#pragma once

//***************************************************************************
#include <math.h>
//***************************************************************************

namespace NGTech
{

	/**
	Bounding sphere
	*/
	class CORE_API BSphere
	{
	public:
		Vec3 center;
		float radius;

		/**
		*/
		inline BSphere()
		{
			center = Vec3(-0.0f, -0.0f, -0.0f);
			radius = 1.0f;
		}

		/**
		*/
		inline BSphere(const Vec3 &center, float radius)
		{
			this->center = center;
			this->radius = radius;
		}

		/**
		*/
		inline BSphere(const BSphere &sphere)
		{
			this->center = sphere.center;
			this->radius = sphere.radius;
		}

		/**
		*/
		inline BSphere &operator=(const BSphere &sphere)
		{
			center = sphere.center;
			radius = sphere.radius;
			return *this;
		}

		/**
		*/
		void AddPoint(const Vec3 &point)
		{
			Vec3 d = point - center;
			float length = d.length();

			if (radius < length)
			{
				radius = length;
			}
		}

		/**
		*/
		void AddSphere(const BSphere &sphere)
		{
			Vec3 dc = sphere.center - center;
			float lc = dc.length();
			float dr = sphere.radius - radius;

			float r = 0.5f * (radius + sphere.radius + lc);
			Vec3 c = (center + dc * (0.5f * (lc + dr) / lc));

			center = c;
			radius = r;
		}

		/**
		*/
		bool IsPointInside(const Vec3 &point)
		{
			return (point - center).length() < radius;
		}

		/**
		*/
		bool IntersectsSphere(const BSphere &sphere)
		{
			return (center - sphere.center).length() <= (radius + sphere.radius);
		}
	};

}
