#pragma once

//***************************************************************************
#include <math.h>
//***************************************************************************

namespace NGTech
{

	/**
	Bounding box
	*/
	class CORE_API BBox
	{
	public:
		Vec3 min, max;

		/**
		*/
		inline BBox()
		{
			min = Vec3(-1.0f, -1.0f, -1.0f);
			max = Vec3(1.0f, 1.0f, 1.0f);
		}

		/**
		*/
		inline BBox(const Vec3 &min, const Vec3 &max)
		{
			this->min = min;
			this->max = max;
		}

		/**
		*/
		inline BBox(const BBox &box)
		{
			this->min = box.min;
			this->max = box.max;
		}

		/**
		*/
		inline BBox &operator=(const BBox &box)
		{
			min = box.min;
			max = box.max;
			return *this;
		}

		/**
		*/
		void AddPoint(const Vec3 &point)
		{
			if (max.x < point.x) max.x = point.x;
			if (max.y < point.y) max.y = point.y;
			if (max.z < point.z) max.z = point.z;

			if (min.x > point.x) min.x = point.x;
			if (min.y > point.y) min.y = point.y;
			if (min.z > point.z) min.z = point.z;
		}

		/**
		*/
		void AddBBox(const BBox &box)
		{
			if (max.x < box.max.x) max.x = box.max.x;
			if (max.y < box.max.y) max.y = box.max.y;
			if (max.z < box.max.z) max.z = box.max.z;

			if (min.x > box.min.x) min.x = box.min.x;
			if (min.y > box.min.y) min.y = box.min.y;
			if (min.z > box.min.z) min.z = box.min.z;
		}

		/**
		*/
		bool IsPointInside(const Vec3 &point)
		{
			return (point.x >= min.x && point.x <= max.x &&
				point.y >= min.y && point.y <= max.y &&
				point.z >= min.z && point.z <= max.z);
		}

		/**
		*/
		Vec3 GetCenter() const
		{
			return (min + max) * 0.5f;
		}
	};

}
