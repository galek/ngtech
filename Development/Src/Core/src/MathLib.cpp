#include "CorePrivate.h"
//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	float Math::clamp(float v, float min, float max) {
		if (v < min) v = min;
		if (v > max) v = max;
		return v;
	}

	float Math::angleBetweenVec(const Vec3 &a, const Vec3 &b) {
		Vec3 va = a;
		Vec3 vb = b;
		float dot = Vec3::dot(va, vb);
		float length = va.length() * vb.length();

		float angle = acos(dot / length);

		if (angle < EPSILON)
			return 0.0f;

		return angle;
	}

	bool Math::intersectPlaneByRay(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &src, const Vec3 &dst, Vec3 &point) {
		Vec3 normal;
		float distance;
		float distance1 = 0, distance2 = 0;

		TBNComputer::computeN(normal, v0, v1, v2);
		distance = -((normal.x * v0.x) + (normal.y * v0.y) + (normal.z * v0.z));

		distance1 = ((normal.x * src.x) + (normal.y * src.y) + (normal.z * src.z)) + distance;
		distance2 = ((normal.x * dst.x) + (normal.y * dst.y) + (normal.z * dst.z)) + distance;

		if (distance1 <= 0) {
			return false;
		}

		Vec3 lineDir;
		double n = 0.0, d = 0.0, dist = 0.0;

		lineDir = Vec3::normalize(dst - src);

		n = -(normal.x * src.x + normal.y * src.y + normal.z * src.z + distance);
		d = Vec3::dot(normal, lineDir);

		if (d == 0.0) {
			point = src;
		}

		dist = n / d;
		point.x = (float)(src.x + (lineDir.x * dist));
		point.y = (float)(src.y + (lineDir.y * dist));
		point.z = (float)(src.z + (lineDir.z * dist));

		return true;
	}

	bool Math::insidePolygon(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &point) {
		const double MATCH_FACTOR = 0.99;
		double angle = 0.0;
		Vec3 a, b;

		a = v0 - point;
		b = v1 - point;
		angle += angleBetweenVec(a, b);

		a = v1 - point;
		b = v2 - point;
		angle += angleBetweenVec(a, b);

		a = v2 - point;
		b = v0 - point;
		angle += angleBetweenVec(a, b);

		if (angle >= (MATCH_FACTOR * (2.0 * PI))) {
			return true;
		}

		return false;
	}


	bool Math::intersectPolygonByRay(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &src, const Vec3 &dst, Vec3 &point) {
		if (!intersectPlaneByRay(v0, v1, v2, src, dst, point))
			return false;

		if (insidePolygon(v0, v1, v2, point))
			return true;

		return false;
	}

	bool Math::intersectSphereByRay(const Vec3 &center, float radius, const Vec3 &src, const Vec3 &dst) {
		Vec3 v1 = center - src;
		Vec3 v2 = Vec3::normalize(dst - src);

		float d = (dst - src).length();
		float t = Vec3::dot(v2, v1);

		if (t <= 0 && (center - src).length() > radius)
			return false;

		Vec3 v3 = v2 * t;
		Vec3 cp = src + v3;

		return (cp - center).length() < radius;
	}

	//---------------------------------------------------------------------------
	//Desc: 2D Vector class
	//---------------------------------------------------------------------------
	Vec2::Vec2() {
		x = y = 0.0;
	}

	Vec2::~Vec2() {}

	Vec2::Vec2(float cx, float cy) {
		x = cx;
		y = cy;
	}

	Vec2::Vec2(const Vec2 &in) {
		x = in.x;
		y = in.y;
	}

	Vec2 &Vec2::operator=(const Vec2 &in) {
		x = in.x;
		y = in.y;
		return *this;
	}

	float &Vec2::operator[](ptrdiff_t index) {
		return *(index + &x);
	}


	float Vec2::operator[](ptrdiff_t index) const {
		return *(index + &x);
	}

	Vec2::operator float*() {
		return &x;
	}

	Vec2::operator const float*() const {
		return &x;
	}

	Vec2 Vec2::operator-() const {
		return Vec2(-x, -y);
	}

	Vec2 Vec2::operator+() const {
		return *this;
	}

	Vec2 &Vec2::operator+=(const Vec2 &v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2 &Vec2::operator-=(const Vec2 &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2 &Vec2::operator*=(const Vec2 &v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vec2 &Vec2::operator/=(const Vec2 &v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}

	bool Vec2::operator==(const Vec2 &v) const {
		return (x == v.x && y == v.y);
	}

	bool Vec2::operator!=(const Vec2 &v) const {
		return (x != v.x || y != v.y);
	}

	ENGINE_INLINE Vec2 operator+(const Vec2 &a, const Vec2 &b) {
		return Vec2(a.x + b.x, a.y + b.y);
	}

	ENGINE_INLINE Vec2 operator-(const Vec2 &a, const Vec2 &b) {
		return Vec2(a.x - b.x, a.y - b.y);
	}

	ENGINE_INLINE Vec2 operator*(const Vec2 &a, const Vec2 &b) {
		return Vec2(a.x * b.x, a.y * b.y);
	}

	ENGINE_INLINE Vec2 operator*(const Vec2 &v, float c) {
		return Vec2(v.x * c, v.y * c);
	}

	ENGINE_INLINE Vec2 operator*(float c, const Vec2 &v) {
		return Vec2(v.x * c, v.y * c);
	}

	ENGINE_INLINE Vec2 operator/(const Vec2 &a, const Vec2 &b) {
		return Vec2(a.x / b.x, a.y / b.y);
	}

	ENGINE_INLINE Vec2 operator/(const Vec2 &v, float c) {
		return Vec2(v.x / c, v.y / c);
	}

	ENGINE_INLINE Vec2 operator/(float c, const Vec2 &v) {
		return Vec2(v.x / c, v.y / c);
	}

	ENGINE_INLINE float Vec2::length() {
		return sqrt(x*x + y*y);
	}

	Vec2 Vec2::normalize(const Vec2 &a) {
		float l = sqrt(a.x*a.x + a.y*a.y);
		if (l < EPSILON)
			return Vec2(0, 0);
		return a / l;
	}
	
	/*
	*/
	Vec3::Vec3() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Vec3::~Vec3() {}

	Vec3::Vec3(float cx, float cy, float cz) {
		x = cx;
		y = cy;
		z = cz;
	}

	Vec3::Vec3(const Vec3 &in) {
		x = in.x;
		y = in.y;
		z = in.z;
	}

	Vec3::Vec3(const Vec4 &in) {
		x = in.x;
		y = in.y;
		z = in.z;
	}

	Vec3 &Vec3::operator=(const Vec3 &in) {
		x = in.x;
		y = in.y;
		z = in.z;
		return *this;
	}

	float& Vec3::operator[](ptrdiff_t index) {
		return *(index + &x);
	}


	float Vec3::operator[](ptrdiff_t index) const {
		return *(index + &x);
	}

	Vec3::operator float*() {
		return &x;
	}

	Vec3::operator const float*() const {
		return &x;
	}

	Vec3 Vec3::operator-() const {
		return Vec3(-x, -y, -z);
	}

	Vec3 Vec3::operator+() const {
		return *this;
	}

	Vec3 &Vec3::operator+=(const Vec3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3 &Vec3::operator-=(const Vec3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vec3 &Vec3::operator*=(const Vec3 &v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vec3 &Vec3::operator/=(const Vec3 &v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	bool Vec3::operator==(const Vec3 &v) const {
		return (x == v.x && y == v.y && z == v.z);
	}


	bool Vec3::operator!=(const Vec3 &v) const {
		return (x != v.x || y != v.y || z != v.z);
	}

	ENGINE_INLINE Vec3 operator+(const Vec3 &a, const Vec3 &b) {
		return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	ENGINE_INLINE Vec3 operator-(const Vec3 &a, const Vec3 &b) {
		return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	ENGINE_INLINE Vec3 operator*(const Vec3 &a, const Vec3 &b) {
		return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	ENGINE_INLINE Vec3 operator*(const Vec3 &v, float c) {
		return Vec3(v.x * c, v.y * c, v.z * c);
	}

	ENGINE_INLINE Vec3 operator*(float c, const Vec3 &v) {
		return Vec3(v.x * c, v.y * c, v.z * c);
	}

	ENGINE_INLINE Vec3 operator/(const Vec3 &a, const Vec3 &b) {
		return Vec3(a.x / b.x, a.y / b.y, a.z / b.z);
	}

	ENGINE_INLINE Vec3 operator/(const Vec3 &v, float c) {
		return Vec3(v.x / c, v.y / c, v.z / c);
	}

	ENGINE_INLINE Vec3 operator/(float c, const Vec3 &v) {
		return Vec3(v.x / c, v.y / c, v.z / c);
	}

	ENGINE_INLINE float Vec3::length() {
		return sqrt(x*x + y*y + z*z);
	}

	Vec3 Vec3::normalize(const Vec3 &a) {
		float l = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
		if (l < EPSILON)
			return Vec3(0, 1, 0);
		return a / l;
	}
	
	ENGINE_INLINE Vec3 Vec3::cross(const Vec3 &a, const Vec3 &b) {
		return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	/*
	*/
	Vec4::Vec4() {
		x = y = z = w = 0.0;
	}

	Vec4::~Vec4() {}

	Vec4::Vec4(float cx, float cy, float cz, float cw) {
		x = cx;
		y = cy;
		z = cz;
		w = cw;
	}

	Vec4::Vec4(const Vec4 &in) {
		x = in.x;
		y = in.y;
		z = in.z;
		w = in.w;
	}

	Vec4::Vec4(const Vec3 &in) {
		x = in.x;
		y = in.y;
		z = in.z;
		w = 1.0;
	}

	Vec4::Vec4(const Vec3 &in, float cw) {
		x = in.x;
		y = in.y;
		z = in.z;
		w = cw;
	}

	Vec4 &Vec4::operator=(const Vec4 &in) {
		x = in.x;
		y = in.y;
		z = in.z;
		w = in.w;
		return *this;
	}

	float& Vec4::operator[](ptrdiff_t index) {
		return *(index + &x);
	}

	float Vec4::operator[](ptrdiff_t index) const {
		return *(index + &x);
	}

	Vec4::operator float*() {
		return &x;
	}

	Vec4::operator const float*() const {
		return &x;
	}

	Vec4 Vec4::operator-() const {
		return Vec4(-x, -y, -z, -w);
	}

	Vec4 Vec4::operator+() const {
		return *this;
	}

	Vec4 &Vec4::operator+=(const Vec4 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vec4 &Vec4::operator-=(const Vec4 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vec4 &Vec4::operator*=(const Vec4 &v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	Vec4 &Vec4::operator/=(const Vec4 &v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	bool Vec4::operator==(const Vec4 &v) const {
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}

	bool Vec4::operator!=(const Vec4 &v) const {
		return (x != v.x || y != v.y || z != v.z || w != v.w);
	}

	ENGINE_INLINE Vec4 operator+(const Vec4 &a, const Vec4 &b) {
		return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	ENGINE_INLINE Vec4 operator-(const Vec4 &a, const Vec4 &b) {
		return Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}


	ENGINE_INLINE Vec4 operator*(const Vec4 &a, const Vec4 &b) {
		return Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	ENGINE_INLINE Vec4 operator*(const Vec4 &v, float c) {
		return Vec4(v.x * c, v.y * c, v.z * c, v.w * c);
	}

	ENGINE_INLINE Vec4 operator*(float c, const Vec4 &v) {
		return Vec4(v.x * c, v.y * c, v.z * c, v.w * c);
	}

	ENGINE_INLINE Vec4 operator/(const Vec4 &a, const Vec4 &b) {
		return Vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
	}

	ENGINE_INLINE Vec4 operator/(const Vec4 &v, float c) {
		return Vec4(v.x / c, v.y / c, v.z / c, v.w / c);
	}

	ENGINE_INLINE Vec4 operator/(float c, const Vec4 &v) {
		return Vec4(v.x / c, v.y / c, v.z / c, v.w / c);
	}

	ENGINE_INLINE float Vec4::length() {
		return sqrt(x*x + y*y + z*z + w*w);
	}

	Vec4 Vec4::normalize(const Vec4 &a) {
		float l = sqrt(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w);
		if (l < EPSILON)
			return Vec4(0, 1, 0, 0);
		return a / l;
	}
	
	/*
	*/
	Mat3::Mat3() {
		Identity();
	}

	Mat3::~Mat3() {}

	Mat3 &Mat3::Identity() {
		e[0] = 1.0f; e[3] = 0.0f; e[6] = 0.0f;
		e[1] = 0.0f; e[4] = 1.0f; e[7] = 0.0f;
		e[2] = 0.0f; e[5] = 0.0f; e[8] = 1.0f;
		return *this;
	}

	Mat3::Mat3(float e0, float e3, float e6,
		float e1, float e4, float e7,
		float e2, float e5, float e8) {
		e[0] = e0; e[3] = e3; e[6] = e6;
		e[1] = e1; e[4] = e4; e[7] = e7;
		e[2] = e2; e[5] = e5; e[8] = e8;
	}

	Mat3::Mat3(const Mat3 &in) {
		e[0] = in.e[0]; e[3] = in.e[3]; e[6] = in.e[6];
		e[1] = in.e[1]; e[4] = in.e[4]; e[7] = in.e[7];
		e[2] = in.e[2]; e[5] = in.e[5]; e[8] = in.e[8];
	}

	Mat3::Mat3(const Mat4 &in) {
		e[0] = in.e[0]; e[3] = in.e[4]; e[6] = in.e[8];
		e[1] = in.e[1]; e[4] = in.e[5]; e[7] = in.e[9];
		e[2] = in.e[2]; e[5] = in.e[6]; e[8] = in.e[10];
	}

	Mat3 &Mat3::operator=(const Mat3 &in) {
		e[0] = in.e[0]; e[3] = in.e[3]; e[6] = in.e[6];
		e[1] = in.e[1]; e[4] = in.e[4]; e[7] = in.e[7];
		e[2] = in.e[2]; e[5] = in.e[5]; e[8] = in.e[8];
		return *this;
	}

	Mat3 &Mat3::operator*=(const Mat3 &in) {
		*this = *this * in;
		return *this;
	}

	float &Mat3::operator[](ptrdiff_t index) {
		return e[index];
	}

	float Mat3::operator[](ptrdiff_t index) const {
		return e[index];
	}

	Mat3::operator float*() {
		return &e[0];
	}

	Mat3::operator const float*() const {
		return &e[0];
	}

	float Mat3::getDeterminant() {
		float d;
		d = e[0] * e[4] * e[8];
		d += e[3] * e[7] * e[2];
		d += e[6] * e[1] * e[5];
		d -= e[6] * e[4] * e[2];
		d -= e[3] * e[1] * e[8];
		d -= e[0] * e[7] * e[5];
		return d;
	}

	Mat3 Mat3::transpose(const Mat3 &m) {
		return Mat3(m.e[0], m.e[1], m.e[2],
			m.e[3], m.e[4], m.e[5],
			m.e[6], m.e[7], m.e[8]);
	}

	Mat3 Mat3::inverse(const Mat3 &m) {
		Mat3 iMat = m;

		float iDet = 1.0f / iMat.getDeterminant();

		iMat.e[0] = (m.e[4] * m.e[8] - m.e[7] * m.e[5]) * iDet;
		iMat.e[1] = -(m.e[1] * m.e[8] - m.e[7] * m.e[2]) * iDet;
		iMat.e[2] = (m.e[1] * m.e[5] - m.e[4] * m.e[2]) * iDet;
		iMat.e[3] = -(m.e[3] * m.e[8] - m.e[6] * m.e[5]) * iDet;
		iMat.e[4] = (m.e[0] * m.e[8] - m.e[6] * m.e[2]) * iDet;
		iMat.e[5] = -(m.e[0] * m.e[5] - m.e[3] * m.e[2]) * iDet;
		iMat.e[6] = (m.e[3] * m.e[7] - m.e[6] * m.e[4]) * iDet;
		iMat.e[7] = -(m.e[0] * m.e[7] - m.e[6] * m.e[1]) * iDet;
		iMat.e[8] = (m.e[0] * m.e[4] - m.e[3] * m.e[1]) * iDet;
		return iMat;
	}

	Mat3 Mat3::rotate(float angle, const Vec3 &axis) {
		float s = sinf(angle * DEG_TO_RAD);
		float c = cosf(angle * DEG_TO_RAD);

		Mat3 rMat;

		float ux = axis.x;
		float uy = axis.y;
		float uz = axis.z;

		rMat.e[0] = c + (1 - c) * ux;
		rMat.e[1] = (1 - c) * ux*uy + s*uz;
		rMat.e[2] = (1 - c) * ux*uz - s*uy;

		rMat.e[3] = (1 - c) * uy*ux - s*uz;
		rMat.e[4] = c + (1 - c) * uy * uy;
		rMat.e[5] = (1 - c) * uy*uz + s*ux;

		rMat.e[6] = (1 - c) * uz*ux + s*uy;
		rMat.e[7] = (1 - c) * uz*uz - s*ux;
		rMat.e[8] = c + (1 - c) * uz * uz;

		return rMat;
	}

	Mat3 Mat3::scale(const Vec3 &scale) {
		Mat3 sMat;

		sMat.e[0] = scale.x;
		sMat.e[4] = scale.y;
		sMat.e[8] = scale.z;

		return sMat;
	}

	ENGINE_INLINE Mat3 operator*(const Mat3 &a, const Mat3 &b) {
		Mat3 result;

		result.e[0] = a.e[0] * b.e[0] + a.e[3] * b.e[1] + a.e[6] * b.e[2];
		result.e[1] = a.e[1] * b.e[0] + a.e[4] * b.e[1] + a.e[7] * b.e[2];
		result.e[2] = a.e[2] * b.e[0] + a.e[5] * b.e[1] + a.e[8] * b.e[2];
		result.e[3] = a.e[0] * b.e[3] + a.e[3] * b.e[4] + a.e[6] * b.e[5];
		result.e[4] = a.e[1] * b.e[3] + a.e[4] * b.e[4] + a.e[7] * b.e[5];
		result.e[5] = a.e[2] * b.e[3] + a.e[5] * b.e[4] + a.e[8] * b.e[5];
		result.e[6] = a.e[0] * b.e[6] + a.e[3] * b.e[7] + a.e[6] * b.e[8];
		result.e[7] = a.e[1] * b.e[6] + a.e[4] * b.e[7] + a.e[7] * b.e[8];
		result.e[8] = a.e[2] * b.e[6] + a.e[5] * b.e[7] + a.e[8] * b.e[8];

		return result;
	}

	ENGINE_INLINE Vec4 operator*(const Mat3 &m, const Vec4 &v) {
		Vec4 result;
		result.x = m.e[0] * v.x + m.e[3] * v.y + m.e[6] * v.z;
		result.y = m.e[1] * v.x + m.e[4] * v.y + m.e[7] * v.z;
		result.z = m.e[2] * v.x + m.e[5] * v.y + m.e[8] * v.z;
		result.w = v.w;
		return result;
	}


	ENGINE_INLINE Vec4 operator*(const Vec4 &v, const Mat3 &m) {
		Vec4 result;
		result.x = m.e[0] * v.x + m.e[3] * v.y + m.e[6] * v.z;
		result.y = m.e[1] * v.x + m.e[4] * v.y + m.e[7] * v.z;
		result.z = m.e[2] * v.x + m.e[5] * v.y + m.e[8] * v.z;
		result.w = v.w;
		return result;
	}

	ENGINE_INLINE Vec3 operator*(const Mat3 &m, const Vec3 &v) {
		Vec3 result;
		result.x = m.e[0] * v.x + m.e[3] * v.y + m.e[6] * v.z;
		result.y = m.e[1] * v.x + m.e[4] * v.y + m.e[7] * v.z;
		result.z = m.e[2] * v.x + m.e[5] * v.y + m.e[8] * v.z;
		return result;
	}


	ENGINE_INLINE Vec3 operator*(const Vec3 &v, const Mat3 &m) {
		Vec3 result;
		result.x = m.e[0] * v.x + m.e[3] * v.y + m.e[6] * v.z;
		result.y = m.e[1] * v.x + m.e[4] * v.y + m.e[7] * v.z;
		result.z = m.e[2] * v.x + m.e[5] * v.y + m.e[8] * v.z;
		return result;
	}
	/*
	*/
	Mat4::Mat4() {
		Identity();
	}

	Mat4::~Mat4() {}

	Mat4 &Mat4::Identity() {
		e[0] = 1.0f; e[4] = 0.0f; e[8] = 0.0f; e[12] = 0.0f;
		e[1] = 0.0f; e[5] = 1.0f; e[9] = 0.0f; e[13] = 0.0f;
		e[2] = 0.0f; e[6] = 0.0f; e[10] = 1.0f; e[14] = 0.0f;
		e[3] = 0.0f; e[7] = 0.0f; e[11] = 0.0f; e[15] = 1.0f;
		return *this;
	}

	Mat4::Mat4(float e0, float e4, float e8, float e12,
		float e1, float e5, float e9, float e13,
		float e2, float e6, float e10, float e14,
		float e3, float e7, float e11, float e15) {
		e[0] = e0; e[4] = e4; e[8] = e8;  e[12] = e12;
		e[1] = e1; e[5] = e5; e[9] = e9;  e[13] = e13;
		e[2] = e2; e[6] = e6; e[10] = e10; e[14] = e14;
		e[3] = e3; e[7] = e7; e[11] = e11; e[15] = e15;
	}

	Mat4::Mat4(const Mat4 &in) {
		e[0] = in.e[0]; e[4] = in.e[4]; e[8] = in.e[8];  e[12] = in.e[12];
		e[1] = in.e[1]; e[5] = in.e[5]; e[9] = in.e[9];  e[13] = in.e[13];
		e[2] = in.e[2]; e[6] = in.e[6]; e[10] = in.e[10]; e[14] = in.e[14];
		e[3] = in.e[3]; e[7] = in.e[7]; e[11] = in.e[11]; e[15] = in.e[15];
	}

	Mat4::Mat4(const Mat3 &in) {
		e[0] = in.e[0]; e[4] = in.e[3]; e[8] = in.e[6];  e[12] = 0.0;
		e[1] = in.e[1]; e[5] = in.e[4]; e[9] = in.e[7];  e[13] = 0.0;
		e[2] = in.e[2]; e[6] = in.e[5]; e[10] = in.e[8]; e[14] = 0.0;
		e[3] = 0.0;  e[7] = 0.0;  e[11] = 0.0;  e[15] = 1.0;
	}

	Mat4 &Mat4::operator=(const Mat4 &in) {
		e[0] = in.e[0]; e[4] = in.e[4]; e[8] = in.e[8];  e[12] = in.e[12];
		e[1] = in.e[1]; e[5] = in.e[5]; e[9] = in.e[9];  e[13] = in.e[13];
		e[2] = in.e[2]; e[6] = in.e[6]; e[10] = in.e[10]; e[14] = in.e[14];
		e[3] = in.e[3]; e[7] = in.e[7]; e[11] = in.e[11]; e[15] = in.e[15];
		return *this;
	}

	Mat4 &Mat4::operator*=(const Mat4 &in) {
		*this = *this * in;
		return *this;
	}

	float &Mat4::operator[](ptrdiff_t index) {
		return e[index];
	}

	float Mat4::operator[](ptrdiff_t index) const {
		return e[index];
	}

	Mat4::operator float*() {
		return &e[0];
	}

	Mat4::operator const float*() const {
		return &e[0];
	}

	float Mat4::getDeterminant() {
		float d;
		d = e[0] * e[5] * e[10];
		d += e[4] * e[9] * e[2];
		d += e[8] * e[1] * e[6];
		d -= e[8] * e[5] * e[2];
		d -= e[4] * e[1] * e[10];
		d -= e[0] * e[9] * e[6];

		return d;
	}

	Vec3 Mat4::getTranslation() {
		Vec3 t;

		t.x = e[12];
		t.y = e[13];
		t.z = e[14];

		return t;
	}

	Mat4 Mat4::getRotation() {
		Mat4 r;

		r.e[0] = e[0]; r.e[4] = e[4]; r.e[8] = e[8];  r.e[12] = 0;
		r.e[1] = e[1]; r.e[5] = e[5]; r.e[9] = e[9];  r.e[13] = 0;
		r.e[2] = e[2]; r.e[6] = e[6]; r.e[10] = e[10]; r.e[14] = 0;
		r.e[3] = 0;    r.e[7] = 0;    r.e[11] = 0;     r.e[15] = 1;

		return r;
	}

	Mat4 Mat4::transpose(const Mat4 &m) {
		return Mat4(m.e[0], m.e[1], m.e[2], m.e[3],
			m.e[4], m.e[5], m.e[6], m.e[7],
			m.e[8], m.e[9], m.e[10], m.e[11],
			m.e[12], m.e[13], m.e[14], m.e[15]);
	}

	Mat4 Mat4::inverse(const Mat4 &m) {
		Mat4 iMat = m;

		float iDet = 1.0f / iMat.getDeterminant();

		iMat.e[0] = (m.e[5] * m.e[10] - m.e[9] * m.e[6]) * iDet;
		iMat.e[1] = -(m.e[1] * m.e[10] - m.e[9] * m.e[2]) * iDet;
		iMat.e[2] = (m.e[1] * m.e[6] - m.e[5] * m.e[2]) * iDet;
		iMat.e[3] = 0.0;

		iMat.e[4] = -(m.e[4] * m.e[10] - m.e[8] * m.e[6]) * iDet;
		iMat.e[5] = (m.e[0] * m.e[10] - m.e[8] * m.e[2]) * iDet;
		iMat.e[6] = -(m.e[0] * m.e[6] - m.e[4] * m.e[2]) * iDet;
		iMat.e[7] = 0.0;

		iMat.e[8] = (m.e[4] * m.e[9] - m.e[8] * m.e[5]) * iDet;
		iMat.e[9] = -(m.e[0] * m.e[9] - m.e[8] * m.e[1]) * iDet;
		iMat.e[10] = (m.e[0] * m.e[5] - m.e[4] * m.e[1]) * iDet;
		iMat.e[11] = 0.0;

		iMat.e[12] = -(m.e[12] * iMat[0] + m.e[13] * iMat[4] + m.e[14] * iMat[8]);
		iMat.e[13] = -(m.e[12] * iMat[1] + m.e[13] * iMat[5] + m.e[14] * iMat[9]);
		iMat.e[14] = -(m.e[12] * iMat[2] + m.e[13] * iMat[6] + m.e[14] * iMat[10]);
		iMat.e[15] = 1.0;

		return iMat;
	}

	Mat4 Mat4::translate(const Vec3 &trans) {
		Mat4 tMat;

		tMat.e[12] = trans.x;
		tMat.e[13] = trans.y;
		tMat.e[14] = trans.z;

		return tMat;
	}

	Mat4 Mat4::rotate(float angle, const Vec3 &axis) {
		float s = sinf(angle * DEG_TO_RAD);
		float c = cosf(angle * DEG_TO_RAD);

		Mat4 rMat;

		float ux = axis.x;
		float uy = axis.y;
		float uz = axis.z;

		rMat.e[0] = c + (1 - c) * ux;
		rMat.e[1] = (1 - c) * ux*uy + s*uz;
		rMat.e[2] = (1 - c) * ux*uz - s*uy;
		rMat.e[3] = 0;

		rMat.e[4] = (1 - c) * uy*ux - s*uz;
		rMat.e[5] = c + (1 - c) * pow(uy, 2);
		rMat.e[6] = (1 - c) * uy*uz + s*ux;
		rMat.e[7] = 0;

		rMat.e[8] = (1 - c) * uz*ux + s*uy;
		rMat.e[9] = (1 - c) * uz*uz - s*ux;
		rMat.e[10] = c + (1 - c) * pow(uz, 2);
		rMat.e[11] = 0;

		rMat.e[12] = 0;
		rMat.e[13] = 0;
		rMat.e[14] = 0;
		rMat.e[15] = 1;

		return rMat;
	}

	Mat4 Mat4::scale(const Vec3 &scale) {
		Mat4 sMat;

		sMat.e[0] = scale.x;
		sMat.e[5] = scale.y;
		sMat.e[10] = scale.z;

		return sMat;
	}

	Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {

		Vec3 x, y, z;
		Mat4 m0, m1;

		z = eye - center;
		z = Vec3::normalize(z);
		x = Vec3::cross(up, z);

		x = Vec3::normalize(x);
		y = Vec3::cross(z, x);
		y = Vec3::normalize(y);

		m0[0] = x.x; m0[4] = x.y; m0[8] = x.z; m0[12] = 0.0;
		m0[1] = y.x; m0[5] = y.y; m0[9] = y.z; m0[13] = 0.0;
		m0[2] = z.x; m0[6] = z.y; m0[10] = z.z; m0[14] = 0.0;
		m0[3] = 0.0; m0[7] = 0.0; m0[11] = 0.0; m0[15] = 1.0;

		m1 = translate(-eye);
		return m0 * m1;
	}

	Mat4 Mat4::perspective(float fovy, float aspect, float n, float f) {
		Mat4 pMat;

		float sine, cotangent, delta_z;
		float radians = (fovy / 2.0f) * (PI / 180.0f);

		delta_z = f - n;
		sine = sinf(radians);
		if ((delta_z == 0) || (sine == 0) || (aspect == 0)) {
			return Mat4();
		}
		cotangent = cosf(radians) / sine;

		pMat.e[0] = cotangent / aspect;
		pMat.e[5] = cotangent;
		pMat.e[10] = -(f + n) / delta_z;
		pMat.e[11] = -1;
		pMat.e[14] = -2 * n * f / delta_z;
		pMat.e[15] = 0;

		return pMat;
	}

	Mat4 Mat4::ortho(float left, float right, float bottom, float top, float n, float f) {
		Mat4 oMat;

		oMat.e[0] = 2.0f / (right - left);

		oMat.e[5] = 2.0f / (top - bottom);

		oMat.e[10] = -2.0f / (f - n);

		oMat.e[12] = -(right + left) / (right - left);
		oMat.e[13] = -(top + bottom) / (top - bottom);
		oMat.e[14] = -(f + n) / (f - n);

		return oMat;
	}

	Mat4 operator*(const Mat4 &a, const Mat4 &b) {
		Mat4 result;

		result.e[0] = (a.e[0] * b.e[0]) + (a.e[4] * b.e[1]) + (a.e[8] * b.e[2]) + (a.e[12] * b.e[3]);
		result.e[1] = (a.e[1] * b.e[0]) + (a.e[5] * b.e[1]) + (a.e[9] * b.e[2]) + (a.e[13] * b.e[3]);
		result.e[2] = (a.e[2] * b.e[0]) + (a.e[6] * b.e[1]) + (a.e[10] * b.e[2]) + (a.e[14] * b.e[3]);
		result.e[3] = (a.e[3] * b.e[0]) + (a.e[7] * b.e[1]) + (a.e[11] * b.e[2]) + (a.e[15] * b.e[3]);

		result.e[4] = (a.e[0] * b.e[4]) + (a.e[4] * b.e[5]) + (a.e[8] * b.e[6]) + (a.e[12] * b.e[7]);
		result.e[5] = (a.e[1] * b.e[4]) + (a.e[5] * b.e[5]) + (a.e[9] * b.e[6]) + (a.e[13] * b.e[7]);
		result.e[6] = (a.e[2] * b.e[4]) + (a.e[6] * b.e[5]) + (a.e[10] * b.e[6]) + (a.e[14] * b.e[7]);
		result.e[7] = (a.e[3] * b.e[4]) + (a.e[7] * b.e[5]) + (a.e[11] * b.e[6]) + (a.e[15] * b.e[7]);

		result.e[8] = (a.e[0] * b.e[8]) + (a.e[4] * b.e[9]) + (a.e[8] * b.e[10]) + (a.e[12] * b.e[11]);
		result.e[9] = (a.e[1] * b.e[8]) + (a.e[5] * b.e[9]) + (a.e[9] * b.e[10]) + (a.e[13] * b.e[11]);
		result.e[10] = (a.e[2] * b.e[8]) + (a.e[6] * b.e[9]) + (a.e[10] * b.e[10]) + (a.e[14] * b.e[11]);
		result.e[11] = (a.e[3] * b.e[8]) + (a.e[7] * b.e[9]) + (a.e[11] * b.e[10]) + (a.e[15] * b.e[11]);

		result.e[12] = (a.e[0] * b.e[12]) + (a.e[4] * b.e[13]) + (a.e[8] * b.e[14]) + (a.e[12] * b.e[15]);
		result.e[13] = (a.e[1] * b.e[12]) + (a.e[5] * b.e[13]) + (a.e[9] * b.e[14]) + (a.e[13] * b.e[15]);
		result.e[14] = (a.e[2] * b.e[12]) + (a.e[6] * b.e[13]) + (a.e[10] * b.e[14]) + (a.e[14] * b.e[15]);
		result.e[15] = (a.e[3] * b.e[12]) + (a.e[7] * b.e[13]) + (a.e[11] * b.e[14]) + (a.e[15] * b.e[15]);

		return result;
	}

	Vec4 operator*(const Mat4 &m, const Vec4 &v) {
		Vec4 result;
		result.x = m.e[0] * v.x + m.e[4] * v.y + m.e[8] * v.z + m.e[12] * v.w;
		result.y = m.e[1] * v.x + m.e[5] * v.y + m.e[9] * v.z + m.e[13] * v.w;
		result.z = m.e[2] * v.x + m.e[6] * v.y + m.e[10] * v.z + m.e[14] * v.w;
		result.w = m.e[3] * v.x + m.e[7] * v.y + m.e[11] * v.z + m.e[15] * v.w;
		return result;
	}


	Vec4 operator*(const Vec4 &v, const Mat4 &m) {
		Vec4 result;
		result.x = m.e[0] * v.x + m.e[4] * v.y + m.e[8] * v.z + m.e[12] * v.w;
		result.y = m.e[1] * v.x + m.e[5] * v.y + m.e[9] * v.z + m.e[13] * v.w;
		result.z = m.e[2] * v.x + m.e[6] * v.y + m.e[10] * v.z + m.e[14] * v.w;
		result.w = m.e[3] * v.x + m.e[7] * v.y + m.e[11] * v.z + m.e[15] * v.w;
		return result;
	}

    Vec3 operator*(const Mat4 &m, const Vec3 &v) {
		Vec3 result;
		result.x = m.e[0] * v.x + m.e[4] * v.y + m.e[8] * v.z + m.e[12];
		result.y = m.e[1] * v.x + m.e[5] * v.y + m.e[9] * v.z + m.e[13];
		result.z = m.e[2] * v.x + m.e[6] * v.y + m.e[10] * v.z + m.e[14];
		return result;
	}

	Vec3 operator*(const Vec3 &v, const Mat4 &m) {
		Vec3 result;
		result.x = m.e[0] * v.x + m.e[4] * v.y + m.e[8] * v.z + m.e[12];
		result.y = m.e[1] * v.x + m.e[5] * v.y + m.e[9] * v.z + m.e[13];
		result.z = m.e[2] * v.x + m.e[6] * v.y + m.e[10] * v.z + m.e[14];
		return result;
	}

	Mat4 Mat4::reflect(const Vec4 &plane) {
		Mat4 out;
		float x = plane.x;
		float y = plane.y;
		float z = plane.z;

		float x2 = x * 2.0f;
		float y2 = y * 2.0f;
		float z2 = z * 2.0f;

		out.e[0] = 1.0f - x * x2;
		out.e[4] = -y * x2;
		out.e[8] = -z * x2;
		out.e[12] = -plane.w * x2;
		out.e[1] = -x * y2;
		out.e[5] = 1.0f - y * y2;
		out.e[9] = -z * y2;
		out.e[13] = -plane.w * y2;
		out.e[2] = -x * z2;
		out.e[6] = -y * z2;
		out.e[10] = 1.0f - z * z2;
		out.e[14] = -plane.w * z2;
		out.e[3] = 0.0;
		out.e[7] = 0.0;
		out.e[11] = 0.0;
		out.e[15] = 1.0;

		return out;
	}

	ENGINE_INLINE float sign(float a) {
		if (a > 0.0) return 1.0;
		if (a < 0.0) return -1.0;
		return 0.0;
	}

	Mat4 Mat4::reflectProjection(const Mat4 &proj, const Vec4 &plane) {
		Mat4 out = proj;

		Vec4 q; //= Vec4(sign(plane.x), 1.0, sign(plane.z), 1.0);
		//q = q * Mat4::inverse(proj);
		q.x = (sign(plane.x) + proj.e[8]) / proj.e[0];
		q.y = (sign(plane.y) + proj.e[9]) / proj.e[5];
		q.z = -1.0;
		q.w = (1.0 + proj.e[10]) / proj.e[14];

		Vec4 c = plane * (2.0 / Vec3::dot(plane, q));

		out.e[2] = c.x;
		out.e[6] = c.y;
		out.e[10] = c.z + 1.0;
		out.e[14] = c.w;

		return out;
	}

	Mat4 Mat4::cube(const Vec3 &position, int face) {
		Mat4 out;

		switch (face)
		{
		case 0: out *= rotate(90.0f, Vec3(0.0f, 1.0f, 0.0f));
			out *= rotate(180.0f, Vec3(1.0f, 0.0f, 0.0f)); break;
		case 1: out *= rotate(-90.0f, Vec3(0.0f, 1.0f, 0.0f));
			out *= rotate(180.0f, Vec3(1.0f, 0.0f, 0.0f)); break;
		case 2: out *= rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f)); break;
		case 3: out *= rotate(90.0f, Vec3(1.0f, 0.0f, 0.0f)); break;
		case 4: out *= rotate(180.0f, Vec3(1.0f, 0.0f, 0.0f)); break;
		case 5: out *= rotate(180.0f, Vec3(1.0f, 0.0f, 0.0f));
			out *= rotate(180.0f, Vec3(0.0f, 1.0f, 0.0f)); break;
		}

		out *= Mat4::translate(-position);
		return out;
	}


	Mat4 Mat4::texBias() {
		return Mat4::translate(Vec3(0.5, 0.5, 1)) *
			Mat4::scale(Vec3(0.5, 0.5, 0));
	}
	/*
	*/
	Quat::Quat() {
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	Quat::Quat(float angle, const Vec3 &axis) {
		Vec3 vdir = axis;
		float length = vdir.length();

		if (length != 0.0) {
			length = 1.0f / length;
			float sinangle = sin(angle * DEG_TO_RAD / 2.0f);
			x = vdir.x * length * sinangle;
			y = vdir.y * length * sinangle;
			z = vdir.z * length * sinangle;
			w = cos(angle * DEG_TO_RAD / 2.0f);

		}
		else {
			x = y = z = 0.0;
			w = 1.0;
		}
	}

	Quat::Quat(const Mat3 &in) {
		float trace = in.e[0] + in.e[4] + in.e[8];

		if (trace > 0.0) {
			float s = sqrt(trace + 1.0f);

			w = 0.5f * s;
			s = 0.5f / s;

			x = (in.e[5] - in.e[7]) * s;
			y = (in.e[6] - in.e[2]) * s;
			z = (in.e[1] - in.e[3]) * s;
		}
		else {
			static int next[3] = { 1, 2, 0 };
			float q[4];

			int i = 0;

			if (in.e[4] > in.e[0])
				i = 1;
			if (in.e[8] > in.e[3 * i + i])
				i = 2;

			int j = next[i];
			int k = next[j];

			float s = sqrt(in.e[3 * i + i] - in.e[3 * j + j] - in.e[3 * k + k] + 1.0f);
			q[i] = 0.5f * s;

			if (s != 0) s = 0.5f / s;

			q[3] = (in.e[3 * j + k] - in.e[3 * k + j]) * s;
			q[j] = (in.e[3 * i + j] + in.e[3 * j + i]) * s;
			q[k] = (in.e[3 * i + k] + in.e[3 * k + i]) * s;

			x = q[0];
			y = q[1];
			z = q[2];
			w = q[3];
		}
	}

	Quat::Quat(float _x, float _y, float _z, float _w)
		:x(_x), y(_y), z(_z), w(_w)
	{}

	Quat::operator float*() {
		return (float*)&x;
	}

	Quat::operator const float*() const {
		return (float*)&x;
	}

	float &Quat::operator[](ptrdiff_t i) {
		return ((float*)&x)[i];
	}

	const float Quat::operator[](ptrdiff_t i) const {
		return ((float*)&x)[i];
	}

	Quat Quat::operator*(const Quat &q) const {
		Quat ret;
		ret.x = w * q.x + x * q.x + y * q.z - z * q.y;
		ret.y = w * q.y + y * q.w + z * q.x - x * q.z;
		ret.z = w * q.z + z * q.w + x * q.y - y * q.x;
		ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
		return ret;
	}

	Quat Quat::slerp(const Quat &q0, const Quat &q1, float t) {
		float k0, k1, cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

		Quat q;
		if (cosomega < 0.0) {
			cosomega = -cosomega;
			q.x = -q1.x;
			q.y = -q1.y;
			q.z = -q1.z;
			q.w = -q1.w;
		}
		else {
			q.x = q1.x;
			q.y = q1.y;
			q.z = q1.z;
			q.w = q1.w;
		}

		if (1.0 - cosomega > 1e-6) {
			float omega = acos(cosomega);
			float sinomega = sin(omega);
			k0 = sin((1.0f - t) * omega) / sinomega;
			k1 = sin(t * omega) / sinomega;
		}
		else {
			k0 = 1.0f - t;
			k1 = t;
		}
		q.x = q0.x * k0 + q.x * k1;
		q.y = q0.y * k0 + q.y * k1;
		q.z = q0.z * k0 + q.z * k1;
		q.w = q0.w * k0 + q.w * k1;

		return q;
	}

	Mat3 Quat::toMatrix() const {
		Mat3 r;
		float x2 = x + x;
		float y2 = y + y;
		float z2 = z + z;
		float xx = x * x2;
		float yy = y * y2;
		float zz = z * z2;
		float xy = x * y2;
		float yz = y * z2;
		float xz = z * x2;
		float wx = w * x2;
		float wy = w * y2;
		float wz = w * z2;

		r[0] = 1.0f - (yy + zz); r[3] = xy - wz;          r[6] = xz + wy;
		r[1] = xy + wz;          r[4] = 1.0f - (xx + zz); r[7] = yz - wx;
		r[2] = xz - wy;          r[5] = yz + wx;          r[8] = 1.0f - (xx + yy);
		return r;
	}
	/*
	*/
	void TBNComputer::computeN(Vec3 &n, Vec3 p0, Vec3 p1, Vec3 p2) {
		Vec3 s = p1 - p0;
		Vec3 t = p2 - p0;
		Vec3 normal = Vec3::cross(s, t);

		n = normal;
	}

	void TBNComputer::computeTBN(Vec3 &t, Vec3 &b, Vec3 p0, Vec3 p1, Vec3 p2, Vec2 t0, Vec2 t1, Vec2 t2, Vec3 n){
		Vec3 position[3];
		Vec2 texcoords[3];

		Vec3 tangent;
		Vec3 binormal;
		Vec3 normal;

		normal = n;
		position[0] = p0;
		position[1] = p1;
		position[2] = p2;

		texcoords[0] = t0;
		texcoords[1] = t1;
		texcoords[2] = t2;

		Vec3 e0(position[1].x - position[0].x,
			texcoords[1].x - texcoords[0].x,
			texcoords[1].y - texcoords[0].y);
		Vec3 e1(position[2].x - position[0].x,
			texcoords[2].x - texcoords[0].x,
			texcoords[2].y - texcoords[0].y);

		Vec3 cp = Vec3::cross(e0, e1);

		if (fabs(cp.x) > EPSILON) {
			tangent.x = -cp.y / cp.x;
			binormal.x = -cp.z / cp.x;
		}
		else {
			tangent.x = 0;
			binormal.x = 0;
		}

		e0.x = position[1].y - position[0].y;
		e1.x = position[2].y - position[0].y;
		cp = Vec3::cross(e0, e1);

		if (fabs(cp.x) > EPSILON) {
			tangent.y = -cp.y / cp.x;
			binormal.y = -cp.z / cp.x;
		}
		else {
			tangent.y = 0;
			binormal.y = 0;
		}

		e0.x = position[1].z - position[0].z;
		e1.x = position[2].z - position[0].z;
		cp = Vec3::cross(e0, e1);

		if (fabs(cp.x) > EPSILON) {
			tangent.z = -cp.y / cp.x;
			binormal.z = -cp.z / cp.x;
		}
		else {
			tangent.z = 0;
			binormal.z = 0;
		}

		Vec3 temp = Vec3(tangent.y * binormal.z - tangent.z * binormal.y,
			tangent.z * binormal.x - tangent.x * binormal.z,
			tangent.x * binormal.y - tangent.y * binormal.x);
		float scalar = normal.x * temp.x + normal.y * temp.y + normal.z * temp.z;
		if (scalar < 0) tangent = -tangent;

		t = tangent;
		b = binormal;
	}

}