#pragma once 

//***************************************************************************
#include "MathLib.h"
#include "../Common/EString.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: GL GLShader struct    
	//---------------------------------------------------------------------------
	class GLShader :public I_Shader {
	public:
		static GLShader *create(const String &path, const String &defines = "");
		virtual~GLShader();

		void set();
		void unset();

		void sendMat4(const String &name, const Mat4 &value);
		void sendVec4(const String &name, const Vec4 &value);
		void sendVec3(const String &name, const Vec3 &value);
		void sendVec2(const String &name, const Vec2 &value);
		void sendFloat(const String &name, float value);
		void sendInt(const String &name, int value);
	};
}