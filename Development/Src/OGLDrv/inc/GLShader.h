#pragma once 

//***************************************************************************
#include "MathLib.h"
#include "../Common/StringHelper.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: GL GLShader struct    
	//---------------------------------------------------------------------------
	class GLShader :public I_Shader {
	public:
		/**
		Creates new GLShader
		\param path shader file path
		\defines shader code defines
		\return pointer to the new GLShader
		*/
		static GLShader *create(const String &path, const String &defines = "");

		/**
		Destroys  GLShader
		*/
		virtual ~GLShader();

		/**
		Sets shader
		*/
		void set();

		/**
		Unsets shader
		*/
		void unset();


		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		void sendMat4(const String &name, const Mat4 &value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		void sendVec4(const String &name, const Vec4 &value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		void sendVec3(const String &name, const Vec3 &value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		void sendVec2(const String &name, const Vec2 &value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		void sendFloat(const String &name, float value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		void sendInt(const String &name, int value);

	private:
		/**
		Creation shader functions
		*/
		bool _createShader(const String &path, const String &defines = "", bool save = true);

		/**
		Saving shader cache in user directory
		*/
		bool _saveCache(const String &path);

		/**
		Creation shader cache user directory
		*/
		const char* _createShaderCacheDirectory(String _file);
	private:
		unsigned int PipelineName;
	};
}