/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//***************************************************************************
#include "Core/IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
#include "Common/EString.h"
//***************************************************************************

namespace VEGA {

//---------------------------------------------------------------------------
//Desc: GL GLShader struct    
//---------------------------------------------------------------------------
class GLShader {
public:
	static GLShader *create(const String &path, const String &defines = "");
	~GLShader();
			
	void set();
	void unset();
	
	void sendMat4(const String &name, const Mat4 &value);
	void sendVec4(const String &name, const Vec4 &value);
	void sendVec3(const String &name, const Vec3 &value);
	void sendVec2(const String &name, const Vec2 &value);
	void sendFloat(const String &name, float value);
	void sendInt(const String &name, int value);

private:
	GLhandleARB gs,vs, fs, cs, tcs, tes, program;
};

};