#include "CommonPrivate.h"
#include "BBox.h"

namespace NGTech
{
	BBox operator*(const Mat4 &a, const BBox &b){
		BBox result;
		result.min = (*a) * b.min;
		result.max = (*a) * b.max;
		return result;
	}
}