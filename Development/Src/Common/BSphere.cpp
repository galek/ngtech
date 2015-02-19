#include "CommonPrivate.h"
#include "BSphere.h"

namespace NGTech
{
	BSphere operator*(const Mat4 &a, const BSphere &s)
	{
		BSphere result;
		result.center = (*a) * s.center;
		result.radius = s.radius;
		return result;
	}
}