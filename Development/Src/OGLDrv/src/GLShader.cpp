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

		if (shader->_createShader(path, defines, true))
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
		glUseProgramObjectARB(program);
		if (PipelineName != 0)
			glBindProgramPipeline(PipelineName);
	}

	void GLShader::unset() {
		glUseProgramObjectARB(NULL);
		glBindProgramPipeline(0);
	}

	void GLShader::sendMat4(const String &name, const Mat4 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		glUniformMatrix4fv(param, 1, false, value);
	}

	void GLShader::sendVec4(const String &name, const Vec4 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		glUniform4fv(param, 1, value);
	}

	void GLShader::sendVec3(const String &name, const Vec3 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		glUniform3fv(param, 1, value);
	}

	void GLShader::sendVec2(const String &name, const Vec2 &value) {
		GLint param = glGetUniformLocation(program, name.c_str());
		glUniform2fv(param, 1, value);
	}

	void GLShader::sendFloat(const String &name, float value) {
		int param = glGetUniformLocation(program, name.c_str());
		glUniform1f(param, value);
	}

	void GLShader::sendInt(const String &name, int value) {
		int param = glGetUniformLocation(program, name.c_str());
		glUniform1i(param, value);
	}

	bool GLShader::_createShader(const String &path, const String &defines, bool _save)
	{
		int Success = 0;

		glGenProgramPipelines(1, &PipelineName);

		{
			this->program = glCreateProgram();
			glProgramParameteri(this->program, GL_PROGRAM_SEPARABLE, GL_TRUE);
			glProgramParameteri(this->program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);


			GLenum Format = 0;
			GLint Size = 0;
			std::vector<unsigned char> Data;

			if (loadBinary(_createShaderCacheDirectory(path.c_str()), Format, Data, Size))
			{
				DebugM("loading shader binary,for %s, is successful", path.c_str());
				glProgramBinary(this->program, Format, &Data[0], Size);
				glGetProgramiv(this->program, GL_LINK_STATUS, &Success);
			}
		}
		DebugM("Validation shader binary is %i", Success);
		if (Success)
			return true;
		else
		{
			if (_save)
				LogPrintf("Shader Cache for shader: %s is not valid", path.c_str());
			VFile mFile(path.c_str(), VFile::READ_TEXT);
			String line, vsCode, fsCode, tcsCode, tesCode, gsCode;

			while (!mFile.IsEof()) {
				line = mFile.GetLine();

				//find GLSL vertex shader
				if (line == "[GLSL_VERTEX_SHADER]") {
					vsCode = "";
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
						char errorString[4096];
						glGetInfoLogARB(this->vs, sizeof(errorString), NULL, errorString);
						Error::showAndExit("GLShader::create() error: shader file '" + path + "' vs compiling error: " + String(errorString));
						return false;
					}
				}

				//find GLSL fragment shader
				if (line == "[GLSL_FRAGMENT_SHADER]") {
					fsCode = "";
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
						char errorString[4096];
						glGetInfoLogARB(this->fs, sizeof(errorString), NULL, errorString);
						Error::showAndExit("GLShader::create() error: shader file '" + path + "' fs compiling error: " + String(errorString));
						return false;
					}
				}

				//find GLSL TESSELATION EVAL SHADER
				if (line == "[GLSL_GEOMETRY_SHADER]") {
					gsCode = "";
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
						char errorString[4096];
						glGetInfoLogARB(this->gs, sizeof(errorString), NULL, errorString);
						Error::showAndExit("GLShader::create() error: shader file '" + path + "' gs compiling error: " + String(errorString));
						return false;
					}
				}


				//find GLSL TESSELATION EVAL SHADER
				if (line == "[GLSL_TESSELATION_EVAL_SHADER]") {
					tesCode = "";
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
						char errorString[4096];
						glGetInfoLogARB(this->tes, sizeof(errorString), NULL, errorString);
						Error::showAndExit("GLShader::create() error: shader file '" + path + "' tes compiling error: " + String(errorString));
						return false;
					}
				}

				//find GLSL TESSELATION CONTROL SHADER
				if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") {
					tcsCode = "";
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
						char errorString[4096];
						glGetInfoLogARB(this->tcs, sizeof(errorString), NULL, errorString);
						Warning("[%s] Error: shader file '%s' tcs compiling error: %s", __FUNCTION__, path.c_str(), (errorString));
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
			glGetInfoLogARB(this->program, sizeof(errorString), NULL, errorString);
			Warning("[%s] Error: shader file '%s' linking error: %s", __FUNCTION__, path, (errorString));
			return false;
		}
		return true;
	}

	bool GLShader::_saveCache(const char* path)
	{
		Debug(__FUNCTION__);
		GLint Size(0);
		GLenum Format(0);

		glGetProgramiv(this->program, GL_PROGRAM_BINARY_LENGTH, &Size);
		std::vector<unsigned char> Data(Size);
		glGetProgramBinary(this->program, Size, nullptr, &Format, &Data[0]);
		return saveBinary(_createShaderCacheDirectory(path), Format, Data, Size);
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
	}