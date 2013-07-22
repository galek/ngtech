/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once
 
//***************************************************************************
#include <math.h>
#include "..\Common\Types.h"
//***************************************************************************

namespace VEGA {

#define PI 3.141592654
#define EPSILON 1e-6f
#define RAD_TO_DEG (180.0f / PI)
#define DEG_TO_RAD (PI / 180.0f)

class Vec2;
class Vec3;
class Vec4;
class Mat4;

//---------------------------------------------------------------------------
//Desc: Some math functions
//---------------------------------------------------------------------------
struct VEGA_API Math {
	static float clamp(float v, float min, float max);

	static float angleBetweenVec(const Vec3 &a, const Vec3 &b);
	static bool insidePolygon(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &point);
	static bool intersectPlaneByRay(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &src, const Vec3 &dst, Vec3 &point);
	static bool intersectPolygonByRay(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &src, const Vec3 &dst, Vec3 &point);
	static bool intersectSphereByRay(const Vec3 &center, float radius, const Vec3 &src, const Vec3 &dst);
};

//---------------------------------------------------------------------------
//Desc: 2D Vector class
//---------------------------------------------------------------------------
class VEGA_API Vec2 {
public:
	float x, y;
	
	Vec2();
	~Vec2();
	Vec2(float cx, float cy);
	Vec2(const Vec2 &in);
	
	Vec2& operator=(const Vec2 &in);
    
	float& operator[](int index);
	float operator[](int index) const;

	operator float*();
    operator const float*() const;

	Vec2 operator-() const;
	Vec2 operator+() const;
	Vec2& operator+=(const Vec2 &v);
	Vec2& operator-=(const Vec2 &v);
	Vec2& operator*=(const Vec2 &v);
	Vec2& operator/=(const Vec2 &v);
	
	bool operator==(const Vec2 &v) const;
    bool operator!=(const Vec2 &v) const;

	EFORCEINLINE float length();

	friend Vec2 operator+(const Vec2 &a, const Vec2 &b);
	friend Vec2 operator-(const Vec2 &a, const Vec2 &b);
	friend Vec2 operator*(const Vec2 &a, const Vec2 &b);
	friend Vec2 operator*(const Vec2 &v, float c);
	friend Vec2 operator*(float c, const Vec2 &v);
	friend Vec2 operator/(const Vec2 &a, const Vec2 &b);
	friend Vec2 operator/(const Vec2 &v, float c);
	friend Vec2 operator/(float c, const Vec2 &v);

	static Vec2 normalize(const Vec2 &a);
	static EFORCEINLINE float dot(const Vec2 &a, const Vec2 &b);
};

extern EFORCEINLINE Vec2 operator+(const Vec2 &a, const Vec2 &b);
extern EFORCEINLINE Vec2 operator-(const Vec2 &a, const Vec2 &b);
extern EFORCEINLINE Vec2 operator*(const Vec2 &a, const Vec2 &b);
extern EFORCEINLINE Vec2 operator*(const Vec2 &v, float c);
extern EFORCEINLINE Vec2 operator*(float c, const Vec2 &v);
extern EFORCEINLINE Vec2 operator/(const Vec2 &a, const Vec2 &b);
extern EFORCEINLINE Vec2 operator/(const Vec2 &v, float c);
extern EFORCEINLINE Vec2 operator/(float c, const Vec2 &v);

//---------------------------------------------------------------------------
//Desc: 3D Vector class
//---------------------------------------------------------------------------
class VEGA_API Vec3 {
public:
	float x, y, z;

	Vec3();
	~Vec3();
	Vec3(float cx, float cy, float cz);
	Vec3(const Vec3 &in);
	Vec3(const Vec4 &in);
	
	Vec3& operator=(const Vec3 &in);
	float& operator[](int index);
	float operator[](int index) const;
	operator float*();
	operator const float*() const;
	
	Vec3 operator-() const;
	Vec3 operator+() const;
	Vec3& operator+=(const Vec3 &v);
	Vec3& operator-=(const Vec3 &v);
	Vec3& operator*=(const Vec3 &v);
	Vec3& operator/=(const Vec3 &v);
	
	bool operator==(const Vec3 &v) const;
	bool operator!=(const Vec3 &v) const;

	EFORCEINLINE float length();

	friend Vec3 operator+(const Vec3 &a, const Vec3 &b);
	friend Vec3 operator-(const Vec3 &a, const Vec3 &b);
	friend Vec3 operator*(const Vec3 &a, const Vec3 &b);
	friend Vec3 operator*(const Vec3 &v, float c);
	friend Vec3 operator*(float c, const Vec3 &v);
	friend Vec3 operator/(const Vec3 &a, const Vec3 &b);
	friend Vec3 operator/(const Vec3 &v, float c);
	friend Vec3 operator/(float c, const Vec3 &v);

	static Vec3 normalize(const Vec3 &a);
	static EFORCEINLINE float dot(const Vec3 &a, const Vec3 &b);
	static EFORCEINLINE Vec3 cross(const Vec3 &a, const Vec3 &b);
};

extern EFORCEINLINE Vec3 operator+(const Vec3 &a, const Vec3 &b);
extern EFORCEINLINE Vec3 operator-(const Vec3 &a, const Vec3 &b);
extern EFORCEINLINE Vec3 operator*(const Vec3 &a, const Vec3 &b);
extern EFORCEINLINE Vec3 operator*(const Vec3 &v, float c);
extern EFORCEINLINE Vec3 operator*(float c, const Vec3 &v);
extern EFORCEINLINE Vec3 operator/(const Vec3 &a, const Vec3 &b);
extern EFORCEINLINE Vec3 operator/(const Vec3 &v, float c);
extern EFORCEINLINE Vec3 operator/(float c, const Vec3 &v);

//---------------------------------------------------------------------------
//Desc: 4D Vector class
//---------------------------------------------------------------------------
class VEGA_API Vec4 {
public:
	float x, y, z, w;
	
	Vec4();
	~Vec4();
	Vec4(float cx, float cy, float cz, float cw);
	Vec4(const Vec4 &in);
	Vec4(const Vec3 &in);
	Vec4(const Vec3 &in, float cw);
	
	Vec4& operator=(const Vec4 &in);
	float& operator[](int index);
	float operator[](int index) const;
	
	operator float*();
	operator const float*() const;
	
	Vec4 operator-() const;
	Vec4 operator+() const;
	
	Vec4& operator+=(const Vec4 &v);
	Vec4& operator-=(const Vec4 &v);
	Vec4& operator*=(const Vec4 &v);
	Vec4& operator/=(const Vec4 &v);
	
	bool operator==(const Vec4 &v) const;
	bool operator!=(const Vec4 &v) const;
	
	EFORCEINLINE float length();

	friend Vec4 operator+(const Vec4 &a, const Vec4 &b);
	friend Vec4 operator-(const Vec4 &a, const Vec4 &b);
	friend Vec4 operator*(const Vec4 &a, const Vec4 &b);
	friend Vec4 operator*(const Vec4 &v, float c);
	friend Vec4 operator*(float c, const Vec4 &v);
	friend Vec4 operator/(const Vec4 &a, const Vec4 &b);
	friend Vec4 operator/(const Vec4 &v, float c);
	friend Vec4 operator/(float c, const Vec4 &v);

	static Vec4 normalize(const Vec4 &a);
	static EFORCEINLINE float dot(const Vec4 &a, const Vec4 &b);
};

extern EFORCEINLINE Vec4 operator+(const Vec4 &a, const Vec4 &b);
extern EFORCEINLINE Vec4 operator-(const Vec4 &a, const Vec4 &b);
extern EFORCEINLINE Vec4 operator*(const Vec4 &a, const Vec4 &b);
extern EFORCEINLINE Vec4 operator*(const Vec4 &v, float c);
extern EFORCEINLINE Vec4 operator*(float c, const Vec4 &v);
extern EFORCEINLINE Vec4 operator/(const Vec4 &a, const Vec4 &b);
extern EFORCEINLINE Vec4 operator/(const Vec4 &v, float c);
extern EFORCEINLINE Vec4 operator/(float c, const Vec4 &v);

//---------------------------------------------------------------------------
//Desc: 3x3 Matrix class
//---------------------------------------------------------------------------
class VEGA_API Mat3 {
public:
	float e[9];
	
	Mat3();
	~Mat3();
	Mat3 &identity();
	Mat3(float e0, float e3, float e6,
          float e1, float e4, float e7,
          float e2, float e5, float e8);
	
	Mat3(const Mat3 &in);
	Mat3(const Mat4 &in);
	
	Mat3 &operator=(const Mat3 &in);
	Mat3 &operator*=(const Mat3 &in);

	float &operator[](int index);
	float operator[](int index) const;
	operator float*();
	operator const float*() const;

	float getDeterminant(); 
	
	static Mat3 transpose(const Mat3 &m);
	static Mat3 inverse(const Mat3 &m);
		
	static Mat3 rotate(float angle, const Vec3 &axis);
	static Mat3 scale(const Vec3 &scale);	
};

extern EFORCEINLINE Mat3 operator*(const Mat3 &a, const Mat3 &b);
extern EFORCEINLINE Vec4 operator*(const Mat3 &m, const Vec4 &v);
extern EFORCEINLINE Vec4 operator*(const Vec4 &v, const Mat3 &m);
extern EFORCEINLINE Vec3 operator*(const Mat3 &m, const Vec3 &v);
extern EFORCEINLINE Vec3 operator*(const Vec3 &v, const Mat3 &m);

//---------------------------------------------------------------------------
//Desc: 4x4 Matrix class
//---------------------------------------------------------------------------
class VEGA_API Mat4 {
public:
	float e[16];
	
	Mat4();
	~Mat4();
	Mat4 &identity();
	Mat4(float e0, float e4, float e8,  float e12,
          float e1, float e5, float e9,  float e13,
          float e2, float e6, float e10, float e14,
          float e3, float e7, float e11, float e15);
	Mat4(const Mat4 &in);
	Mat4(const Mat3 &in);
	
	Mat4 &operator=(const Mat4 &in);
	Mat4 &operator*=(const Mat4 &in);

	float &operator[](int index);
	float operator[](int index) const;
	operator float*();
	operator const float*() const;

	Vec3 getTranslation();
	Mat4 getRotation();
	float getDeterminant(); 
	
	static Mat4 transpose(const Mat4 &m);
	static Mat4 inverse(const Mat4 &m);
		
	static Mat4 translate(const Vec3 &trans);
	static Mat4 rotate(float angle, const Vec3 &axis);
	static Mat4 scale(const Vec3 &scale);	
	static Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
	
	static Mat4 perspective(float fovy, float aspect, float n, float f);
	static Mat4 ortho(float left, float right, float bottom, float top, float n, float f);

	static Mat4 cube(const Vec3 &position, int face);

	static Mat4 texBias();
};

extern EFORCEINLINE Mat4 operator*(const Mat4 &a, const Mat4 &b);
extern EFORCEINLINE Vec4 operator*(const Mat4 &m, const Vec4 &v);
extern EFORCEINLINE Vec4 operator*(const Vec4 &v, const Mat4 &m);
extern EFORCEINLINE Vec3 operator*(const Mat4 &m, const Vec3 &v);
extern EFORCEINLINE Vec3 operator*(const Vec3 &v, const Mat4 &m);
			
//---------------------------------------------------------------------------
//Desc: Quat class
//---------------------------------------------------------------------------
class VEGA_API Quat {
public:
	float x,y,z,w;

	Quat();
	Quat(float angle, const Vec3 &axis);		
	
	Quat(const Mat3 &in);
	
	operator float*();
	operator const float*() const;
	
	float &operator[](int i);
	const float operator[](int i) const;
	
	Quat operator*(const Quat &q) const;
	static Quat slerp(const Quat &q0, const Quat &q1, float t);
	Mat3 toMatrix() const;
};

//---------------------------------------------------------------------------
//Desc: Computes TBN basis
//---------------------------------------------------------------------------
struct VEGA_API TBNComputer {
	static void computeN(Vec3 &n, Vec3 p0, Vec3 p1, Vec3 p2);
	static void computeTBN(Vec3 &t, Vec3 &b, Vec3 p0, Vec3 p1, Vec3 p2, Vec2 t0, Vec2 t1, Vec2 t2, Vec3 n);
};

};