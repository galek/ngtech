/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    GLShader.cpp
* Desc:    Shader manager impl.
* Version: 1.01
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#include "RenderPrivate.h"
//***************************************************************************
#include <direct.h> //_mkdir
//***************************************************************************
#include "GLSystem.h"
#include "GLShader.h"
#include "Log.h"
#include "FileHelper.h"
#include "Error.h"
#include "vfs.h"
//***************************************************************************

namespace NGTech {

	GLShader *GLShader::create(const String &path, const String &defines) {
		GLShader *shader = new GLShader();

		if (shader->CreateShader(path, defines, true))
			return shader;
		else
			return NULL;
	}

	GLShader::GLShader() :
		Filename("NULL")
	{
		this->vs = NULL;
		this->fs = NULL;
		this->cs = NULL;
		this->tcs = NULL;
		this->tes = NULL;
		this->program = NULL;
	}

	GLShader::~GLShader() {
		Release();
	}

	void GLShader::Release()
	{
		if (vs) glDeleteProgram(vs);
		if (fs) glDeleteProgram(fs);
		if (gs) glDeleteProgram(gs);
		if (cs) glDeleteProgram(cs);
		if (tcs) glDeleteProgram(tcs);
		if (tes) glDeleteProgram(tes);
		if (program) glDeleteProgram(program);
		if (PipelineName) glDeleteProgramPipelines(1, &PipelineName);
	}

	void GLShader::set() {
		glUseProgram(program);
		if (PipelineName != 0)
			glBindProgramPipeline(PipelineName);
	}

	void GLShader::unset() {
		glUseProgram(NULL);
		glBindProgramPipeline(0);
	}

	void GLShader::sendMat4(const String &name, const Mat4 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		_uniformLocationList[name] = param;
		glUniformMatrix4fv(param, 1, false, value);
	}

	void GLShader::sendVec4(const String &name, const Vec4 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		_uniformLocationList[name] = param;
		glUniform4fv(param, 1, value);
	}

	void GLShader::sendVec3(const String &name, const Vec3 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		_uniformLocationList[name] = param;
		glUniform3fv(param, 1, value);
	}

	void GLShader::sendVec2(const String &name, const Vec2 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		_uniformLocationList[name] = param;
		glUniform2fv(param, 1, value);
	}

	void GLShader::sendFloat(const String &name, float value) {
		int param = glGetUniformLocation(program, name.c_str());
		_uniformLocationList[name] = param;
		glUniform1f(param, value);
	}

	void GLShader::sendInt(const String &name, size_t value) {
		int param = glGetUniformLocation(program, name.c_str());
		_uniformLocationList[name] = param;
		glUniform1i(param, value);
	}

	bool GLShader::CreateShader(const String &path, const String &defines, bool _save)
	{
		int Success = 0;

		glGenProgramPipelines(1, &PipelineName);

#ifndef _ENGINE_DEBUG_
		{
			GLenum Format = 0;
			GLint Size = 0;
			std::vector<unsigned char> Data;

			if (loadBinary(_createShaderCacheDirectory(path.c_str()), Format, Data, Size))
			{
				this->program = glCreateProgram();
				glProgramParameteri(this->program, GL_PROGRAM_SEPARABLE, GL_TRUE);
				glProgramParameteri(this->program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

				DebugM("loading shader binary,for %s, is successful", path.c_str());
				glProgramBinary(this->program, Format, &Data[0], Size);
				glGetProgramiv(this->program, GL_LINK_STATUS, &Success);
			}
		}
		DebugM("Validation shader binary is %i", Success);
		if (Success)
			return true;
		else
#endif
		{
			if (_save)
				LogPrintf("Shader Cache for shader: %s is not valid", path.c_str());
			VFile mFile(path.c_str(), VFile::READ_TEXT);
			String line, vsCode = "", fsCode = "", tcsCode = "", tesCode = "", gsCode = "";

			while (!mFile.IsEof()) {
				line = mFile.GetLine();

				//find GLSL vertex shader
				if (line == "[GLSL_VERTEX_SHADER]") {
					while (!mFile.IsEof()) {
						line = mFile.GetLine();;
						if (line == "[GLSL_FRAGMENT_SHADER]") break;
						else if (line == "[GLSL_COMPUTE_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_EVAL_SHADER]") break;
						else if (line == "[GLSL_GEOMETRY_SHADER]") break;
						vsCode = vsCode + line + "\n";
					}

					vsCode = defines + vsCode;

					const char *vsString[1];
					vsString[0] = (char*)vsCode.c_str();

					this->vs = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
					glShaderSource(this->vs, 1, vsString, NULL);
					glCompileShader(this->vs);

					int compiled;
					glGetShaderiv(this->vs, GL_COMPILE_STATUS, &compiled);

					if (!compiled) {
						char errorString[1024] = { 0 };
						glGetProgramInfoLog(this->vs, sizeof(errorString), NULL, errorString);
						Warning("[%s] Error: shader file '%s' vs compiling error: %s", __FUNCTION__, path.c_str(), String(errorString));
						Error("Failed compiling shader", true);
						return false;
					}
				}

				//find GLSL fragment shader
				if (line == "[GLSL_FRAGMENT_SHADER]") {
					while (!mFile.IsEof()) {
						line = mFile.GetLine();
						if (line == "[GLSL_VERTEX_SHADER]") break;
						else if (line == "[GLSL_COMPUTE_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_EVAL_SHADER]") break;
						else if (line == "[GLSL_GEOMETRY_SHADER]") break;
						fsCode = fsCode + line + "\n";
					}

					fsCode = defines + fsCode;

					const char *fsString[1];
					fsString[0] = (char*)fsCode.c_str();

					this->fs = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
					glShaderSource(this->fs, 1, fsString, NULL);
					glCompileShader(this->fs);

					int compiled;
					glGetShaderiv(this->fs, GL_COMPILE_STATUS, &compiled);

					if (!compiled) {
						char errorString[1000] = { 0 };
						glGetProgramInfoLog(this->fs, sizeof(errorString), 0, errorString);
						Warning("[%s] Error: shader file '%s' fs compiling error: %s", __FUNCTION__, path.c_str(), String(errorString));
						Error("Failed compiling shader", true);
						return false;
					}
				}

				//find GLSL TESSELATION EVAL SHADER
				if (line == "[GLSL_GEOMETRY_SHADER]") {
					while (!mFile.IsEof()) {
						line = mFile.GetLine();
						if (line == "[GLSL_FRAGMENT_SHADER]") break;
						else if (line == "[GLSL_COMPUTE_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_EVAL_SHADER]") break;
						else if (line == "[GLSL_VERTEX_SHADER]") break;
						gsCode = gsCode + line + "\n";
					}

					gsCode = defines + gsCode;

					const char *gsString[1];
					gsString[0] = (char*)gsCode.c_str();

					this->gs = glCreateShaderObjectARB(GL_GEOMETRY_SHADER);
					glShaderSourceARB(this->gs, 1, gsString, NULL);
					glCompileShaderARB(this->gs);

					int compiled;
					glGetShaderiv(this->gs, GL_COMPILE_STATUS, &compiled);

					if (!compiled) {
						char errorString[1024] = { 0 };
						glGetProgramInfoLog(this->gs, sizeof(errorString), NULL, errorString);
						Warning("[%s] Error: shader file '%s' gs compiling error: %s", __FUNCTION__, path.c_str(), String(errorString));
						Error("Failed compiling shader", true);
						return false;
					}
				}


				//find GLSL TESSELATION EVAL SHADER
				if (line == "[GLSL_TESSELATION_EVAL_SHADER]") {
					while (!mFile.IsEof()) {
						line = mFile.GetLine();
						if (line == "[GLSL_FRAGMENT_SHADER]") break;
						else if (line == "[GLSL_COMPUTE_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") break;
						else if (line == "[GLSL_VERTEX_SHADER]") break;
						else if (line == "[GLSL_GEOMETRY_SHADER]") break;
						tesCode = tesCode + line + "\n";
					}

					tesCode = defines + tesCode;

					const char *tesString[1];
					tesString[0] = (char*)tesCode.c_str();

					this->tes = glCreateShaderObjectARB(GL_TESS_EVALUATION_SHADER);
					glShaderSourceARB(this->tes, 1, tesString, NULL);
					glCompileShaderARB(this->tes);

					int compiled;
					glGetShaderiv(this->tes, GL_COMPILE_STATUS, &compiled);

					if (!compiled) {
						char errorString[1024] = { 0 };
						glGetProgramInfoLog(this->tes, sizeof(errorString), NULL, errorString);
						Warning("[%s] Error: shader file '%s' tes compiling error: %s", __FUNCTION__, path.c_str(), String(errorString));
						Error("Failed compiling shader", true);
						return false;
					}
				}

				//find GLSL TESSELATION CONTROL SHADER
				if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") {
					while (!mFile.IsEof()) {
						line = mFile.GetLine();
						if (line == "[GLSL_FRAGMENT_SHADER]") break;
						else if (line == "[GLSL_COMPUTE_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_EVAL_SHADER]") break;
						else if (line == "[GLSL_VERTEX_SHADER]") break;
						else if (line == "[GLSL_GEOMETRY_SHADER]") break;
						tcsCode = tcsCode + line + "\n";
					}

					tcsCode = defines + tcsCode;

					const char *tecString[1];
					tecString[0] = (char*)tcsCode.c_str();

					this->vs = glCreateShaderObjectARB(GL_TESS_CONTROL_SHADER);
					glShaderSourceARB(this->tcs, 1, tecString, NULL);
					glCompileShaderARB(this->tcs);

					int compiled;
					glGetShaderiv(this->tcs, GL_COMPILE_STATUS, &compiled);

					if (!compiled) {
						char errorString[1024] = { 0 };
						glGetProgramInfoLog(this->tcs, sizeof(errorString), NULL, errorString);
						Warning("[%s] Error: shader file '%s' tcs compiling error: %s", __FUNCTION__, path.c_str(), String(errorString));
						Error("Failed compiling shader", true);
						return false;
					}
				}
			}

			//create
			this->program = glCreateProgram();
			if (this->vs)  glAttachShader(this->program, this->vs);
			if (this->fs)  glAttachShader(this->program, this->fs);
			if (this->gs)  glAttachShader(this->program, this->gs);
			if (this->tes) glAttachShader(this->program, this->tes);
			if (this->tcs) glAttachShader(this->program, this->tcs);

			if (!_checkLinked(path.c_str()))
				return false;

			if (_save)
			{
				if (!_saveCache(path.c_str()))
				{
					Warning("Failed Saving Compiled cache: %s", path.c_str());
				}
			}
			return true;
		}
		return false;
	}

	bool GLShader::_checkLinked(const char*path)
	{
		int linked;
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_LINK_STATUS, &linked);

		if (!linked) {
			char errorString[4096];
			glGetProgramInfoLog(this->program, sizeof(errorString), NULL, errorString);
			Warning("[%s] Error: shader file '%s' linking error: %s", __FUNCTION__, path, (errorString));
			return false;
		}
		return true;
	}

	bool GLShader::_saveCache(const char* path)
	{
#ifndef _ENGINE_DEBUG_
		Debug(__FUNCTION__);
		GLint Size(0);
		GLenum Format(0);

		glGetProgramiv(this->program, GL_PROGRAM_BINARY_LENGTH, &Size);
		std::vector<unsigned char> Data(Size);
		glGetProgramBinary(this->program, Size, nullptr, &Format, &Data[0]);
		return saveBinary(_createShaderCacheDirectory(path), Format, Data, Size);
#else
		return true;
#endif
	}

	const char* GLShader::_createShaderCacheDirectory(const char * _file)
	{
#if PLATFORM_MEMORY_ADDRESS_SPACE == PLATFORM_MEMORY_ADDRESS_SPACE_32BIT
		static const char* dir = "../userData/ShaderCache.32/";
#else
		static const char* dir = "../userData/ShaderCache.64/";
#endif
		_mkdir(dir);
		Filename = dir;
		Filename += _file;
		Filename += ".bin";
		return Filename.c_str();
	}

	/**
	*/
	int GLShader::GetUniformLocation(const char*uniform, bool isOptional)
	{
		GLint result = glGetUniformLocation(this->program, uniform);
		_uniformLocationList[uniform] = result;

		if (result == -1)
		{
			std::string strErr("could not find uniform \"%s\" in program %d");
			strErr += uniform;
			strErr += this->program;

			if (!isOptional){
				Error(strErr.c_str(), true);
			}
			Warning(strErr.c_str());
		}
		return result;
	}

	/**
	*/
	int GLShader::GetAttribLocation(const char* attribute, bool isOptional)
	{
		GLint result = glGetAttribLocation(this->program, attribute);
		_attributeList[attribute] = result;

		if (result == -1)
		{
			std::string strErr("could not find uniform \"%s\" in program %d");
			strErr += attribute;
			strErr += this->program;

			if (!isOptional){
				Error(strErr.c_str(), true);
			}
			Warning(strErr.c_str());
		}

		return result;
	}

	/**
	*/
	void GLShader::BindTexture2D(const char *name, int unit, unsigned int tex)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform1i(loc, unit);
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
	}

	/**
	*/
	void GLShader::BindTexture2D(int index, int unit, unsigned int tex)
	{
		glUniform1i(index, unit);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, tex);
	}

	/**
	*/
	void GLShader::BindTextureRect(const char *name, int unit, unsigned int tex)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform1i(loc, unit);
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(0x84F5/*GL_TEXTURE_RECT*/, tex);
		}
	}

	/**
	*/
	void GLShader::BindTextureRect(int index, int unit, unsigned int tex)
	{
		glUniform1i(index, unit);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(0x84F5/*GL_TEXTURE_RECT*/, tex);
	}

	/**
	*/
	void GLShader::BindTextureArray(const char *name, int unit, unsigned int tex)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform1i(loc, unit);
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(0x8c1a, tex); // GL_TEXTURE_2D_ARRAY
		}
	}

	/**
	*/
	void GLShader::BindTextureArray(int index, int unit, unsigned int tex)
	{
		glUniform1i(index, unit);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(0x8c1a, tex); // GL_TEXTURE_2D_ARRAY
	}

	/**
	*/
	void GLShader::SetUniform1i(const char *name, int value)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform1i(loc, value);
		}
	}

	/**
	*/
	void GLShader::SetUniform1i(int index, int value)
	{
		if (index >= 0) {
			glUniform1i(index, value);
		}
	}

	/**
	*/
	void GLShader::SetUniform2i(const char *name, int x, int y)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform2i(loc, x, y);
		}
	}

	/**
	*/
	void GLShader::SetUniform2i(int index, int x, int y)
	{
		if (index >= 0) {
			glUniform2i(index, x, y);
		}
	}

	/**
	*/
	void GLShader::SetUniform3i(const char *name, int x, int y, int z)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform3i(loc, x, y, z);
		}
	}

	/**
	*/
	void GLShader::SetUniform3i(int index, int x, int y, int z)
	{
		if (index >= 0) {
			glUniform3i(index, x, y, z);
		}
	}

	/**
	*/
	void GLShader::SetUniform1f(const char *name, float value)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform1f(loc, value);
		}
	}

	/**
	*/
	void GLShader::SetUniform1f(int index, float value)
	{
		if (index >= 0) {
			glUniform1f(index, value);
		}
	}

	/**
	*/
	void GLShader::SetUniform2f(const char *name, float x, float y)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform2f(loc, x, y);
		}
	}

	/**
	*/
	void GLShader::SetUniform2f(int index, float x, float y)
	{
		if (index >= 0) {
			glUniform2f(index, x, y);
		}
	}

	/**
	*/
	void GLShader::SetUniform3f(const char *name, float x, float y, float z)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform3f(loc, x, y, z);
		}
	}

	/**
	*/
	void GLShader::SetUniform3f(int index, float x, float y, float z)
	{
		if (index >= 0) {
			glUniform3f(index, x, y, z);
		}
	}

	/**
	*/
	void GLShader::SetUniform4f(const char *name, float x, float y, float z, float w)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform4f(loc, x, y, z, w);
		}
	}

	/**
	*/
	void GLShader::SetUniform4f(int index, float x, float y, float z, float w)
	{
		if (index >= 0) {
			glUniform4f(index, x, y, z, w);
		}
	}

	/**
	*/
	void GLShader::SetUniform3fv(const char *name, const float *value, int count)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform3fv(loc, count, value);
		}

	}

	/**
	*/
	void GLShader::SetUniform3fv(int index, const float *value, int count)
	{
		if (index >= 0) {
			glUniform3fv(index, count, value);
		}
	}

	/**
	*/
	void GLShader::SetUniform4fv(const char *name, const float *value, int count)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniform4fv(loc, count, value);
		}
	}

	/**
	*/
	void GLShader::SetUniform4fv(int index, const float *value, int count)
	{
		if (index >= 0) {
			glUniform4fv(index, count, value);
		}
	}

	/**
	*/
	void GLShader::SetUniformMatrix4fv(const char *name, float *m, int count, bool transpose)
	{
		GLint loc = GetUniformLocation(name, false);
		if (loc >= 0) {
			glUniformMatrix4fv(loc, count, transpose, m);
		}
	}

	/**
	*/
	void GLShader::SetUniformMatrix4fv(int index, float *m, int count, bool transpose)
	{
		if (index >= 0) {
			glUniformMatrix4fv(index, count, transpose, m);
		}
	}

	/**
	*/
	void GLShader::AddAttribute(const std::string& attribute) {
		_attributeList[attribute] = glGetAttribLocation(this->program, attribute.c_str());
	}

	/**
	*/
	void GLShader::AddUniform(const std::string& uniform) {
		_uniformLocationList[uniform] = glGetUniformLocation(this->program, uniform.c_str());
	}

	/**
	*/
	GLuint GLShader::operator [](const std::string& attribute) {
		return _attributeList[attribute];
	}

	/**
	*/
	GLuint GLShader::operator()(const std::string& uniform){
		return _uniformLocationList[uniform];
	}

	/**
	*/
	GLuint GLShader::operator [](const char* attribute) {
		return _attributeList[attribute];
	}

	/**
	*/
	GLuint GLShader::operator()(const char* uniform){
		return _uniformLocationList[uniform];
	}
}