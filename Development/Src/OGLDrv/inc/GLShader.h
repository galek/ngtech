#pragma once 

//***************************************************************************
#include "../../Core/Inc/MathLib.h"
#include "../../Common/StringHelper.h"
//***************************************************************************
#include "Renderdll.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: GL GLShader struct    
	//---------------------------------------------------------------------------
	class RENDER_API GLShader : public I_Shader {
	public:
		/**
		Creates new GLShader
		\param path shader file path
		\defines shader code defines
		\return pointer to the new GLShader
		*/
		static GLShader *create(const String &path, const String &defines = "");

		/**
		*/
		GLShader();

		/**
		Destroys  GLShader
		*/
		virtual ~GLShader();

		/**
		Sets shader
		*/
		virtual void set();

		/**
		Unsets shader
		*/
		virtual void unset();

		/**
		Release shader
		*/
		virtual void Release();
		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		virtual void sendMat4(const String &name, const Mat4 &value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		virtual void sendVec4(const String &name, const Vec4 &value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		virtual void sendVec3(const String &name, const Vec3 &value);

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
		virtual void sendFloat(const String &name, float value);

		/**
		Sends uniform param to shader
		\param name parameter name
		\param value param value
		*/
		virtual void sendInt(const String &name, size_t value);

		/**
		*/
		virtual int GetUniformLocation(const char*_loc, bool isOptional);

		/**
		*/
		virtual int GetAttribLocation(const char* attribute, bool isOptional);

		/**
		*/
		virtual void BindTexture2D(const char *name, int unit, unsigned int tex);

		/**
		*/
		virtual void BindTexture2D(int index, int unit, unsigned int tex);

		/**
		*/
		virtual void BindTextureRect(const char *name, int unit, unsigned int tex);

		/**
		*/
		virtual void BindTextureRect(int index, int unit, unsigned int tex);

		/**
		*/
		virtual void BindTextureArray(const char *name, int unit, unsigned int tex);

		/**
		*/
		virtual void BindTextureArray(int index, int unit, unsigned int tex);

		/**
		*/
		virtual void SetUniform1i(const char *name, int value);

		/**
		*/
		virtual void SetUniform1i(int index, int value);

		/**
		*/
		virtual void SetUniform2i(const char *name, int x, int y);

		/**
		*/
		virtual void SetUniform2i(int index, int x, int y);

		/**
		*/
		virtual void SetUniform3i(const char *name, int x, int y, int z);

		/**
		*/
		virtual void SetUniform3i(int index, int x, int y, int z);

		/**
		*/
		virtual void SetUniform1f(const char *name, float value);

		/**
		*/
		virtual void SetUniform1f(int index, float value);

		/**
		*/
		virtual void SetUniform2f(const char *name, float x, float y);

		/**
		*/
		virtual void SetUniform2f(int index, float x, float y);

		/**
		*/
		virtual void SetUniform3f(const char *name, float x, float y, float z);

		/**
		*/
		virtual void SetUniform3f(int index, float x, float y, float z);

		/**
		*/
		virtual void SetUniform4f(const char *name, float x, float y, float z, float w);

		/**
		*/
		virtual void SetUniform4f(int index, float x, float y, float z, float w);

		/**
		*/
		virtual void SetUniform3fv(const char *name, const float *value, int count);

		/**
		*/
		virtual void SetUniform3fv(int index, const float *value, int count);

		/**
		*/
		virtual void SetUniform4fv(const char *name, const float *value, int count);

		/**
		*/
		virtual void SetUniform4fv(int index, const float *value, int count);

		/**
		*/
		virtual void SetUniformMatrix4fv(const char *name, float *m, int count, bool transpose);

		/**
		*/
		virtual void SetUniformMatrix4fv(int index, float *m, int count, bool transpose);

		/**
		*/
		virtual void AddAttribute(const std::string& attribute);

		/**
		*/
		virtual void AddUniform(const std::string& uniform);
		/**
		Creation shader functions
		*/
		virtual bool CreateShader(const String &path, const String &defines = "", bool save = true);
		/**
		An indexer that returns the location of the attribute/uniform
		*/
		unsigned int operator[](const std::string& attribute);
		/**
		*/
		unsigned int operator()(const std::string& uniform);
		/**
		An indexer that returns the location of the attribute/uniform
		*/
		unsigned int operator[](const char* attribute);
		/**
		*/
		unsigned int operator()(const char* uniform);
	private:

		/**
		Saving shader cache in user directory
		*/
		bool _saveCache(const char* path);

		/**
		Creation shader cache user directory
		*/
		const char* _createShaderCacheDirectory(const char* _file);

		bool _checkLinked(const char*path);
	private:
		unsigned int PipelineName, gs, vs, fs, cs, tcs, tes, program;
		std::string Filename;
		std::map<std::string, unsigned int> _attributeList;
		std::map<std::string, unsigned int> _uniformLocationList;
	};
}