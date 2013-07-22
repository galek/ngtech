/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLShader.h"
#include "Log.h"
#include "File.h"
#include "Error.h"
#include "vfs.h"
//***************************************************************************

namespace VEGA {

	//---------------------------------------------------------------------------
	//Desc:    creates GLShader from file
	//Params:  path - shader file path, defines - shader source defines string
	//Returns: pointer to new GLShader
	//---------------------------------------------------------------------------
	GLShader *GLShader::create(const String &path, const String &defines) {
		GLShader *shader = new GLShader();

		shader->vs = NULL;
		shader->fs = NULL;
		shader->cs = NULL;
		shader->tcs = NULL;
		shader->tes = NULL;
		shader->program = NULL;

		if(!engine.vfs->isDataExist(path)) {
			Error::showAndExit("GLShader::create() error: shader file '" + path + "' was not found");
			return NULL;
		}
		
		FILE *shdFile = fopen(engine.vfs->getDataPath(path).c_str(), "rt");
		String line, vsCode, fsCode;

		while(!feof(shdFile)) {
			line = FileHelper::readString(shdFile);


			//find GLSL vertex shader
			if(line == "[GLSL_VERTEX_SHADER]") {
				vsCode = "";
				while(!feof(shdFile)) {
					line = FileHelper::readString(shdFile);
					if (line == "[GLSL_FRAGMENT_SHADER]") break;
					else if (line == "[GLSL_COMPUTE_SHADER]") break;
					else if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") break;
					else if (line == "[GLSL_TESSELATION_EVAL_SHADER]") break;
					vsCode = vsCode + line + "\n";
				}

				vsCode = defines + vsCode;

				const char *vsString[1];
				vsString[0] = (char*)vsCode.c_str();

				shader->vs = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
				glShaderSourceARB(shader->vs, 1, vsString, NULL);
				glCompileShaderARB(shader->vs);

				int compiled;
				glGetObjectParameterivARB(shader->vs, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);

				if(!compiled) {
					char errorString[4096];
					glGetInfoLogARB(shader->vs, sizeof(errorString), NULL, errorString);
					Error::showAndExit("GLShader::create() error: shader file '" + path + "' vs compiling error: " + String(errorString));
					return NULL;
				}
			}


			//find GLSL fragment shader
			if(line == "[GLSL_FRAGMENT_SHADER]") {
				fsCode = "";
				while(!feof(shdFile)) {
					line = FileHelper::readString(shdFile);
					if(line == "[GLSL_VERTEX_SHADER]") break;
					else if (line == "[GLSL_COMPUTE_SHADER]") break;
					else if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") break;
					else if (line == "[GLSL_TESSELATION_EVAL_SHADER]") break;
					fsCode = fsCode + line + "\n";
				}

				fsCode = defines + fsCode;

				const char *fsString[1];
				fsString[0] = (char*)fsCode.c_str();

				shader->fs = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
				glShaderSourceARB(shader->fs, 1, fsString, NULL);
				glCompileShaderARB(shader->fs);

				int compiled;
				glGetObjectParameterivARB(shader->fs, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);

				if(!compiled) {
					char errorString[4096];
					glGetInfoLogARB(shader->fs, sizeof(errorString), NULL, errorString);
					Error::showAndExit("GLShader::create() error: shader file '" + path + "' fs compiling error: " + String(errorString));
					return NULL;
				}
			}
		}

		fclose(shdFile);

		//create
		shader->program  = glCreateProgramObjectARB();
		if(shader->vs) glAttachObjectARB(shader->program, shader->vs);
		if(shader->fs) glAttachObjectARB(shader->program, shader->fs);

		int linked;
		glLinkProgramARB(shader->program);
		glGetObjectParameterivARB(shader->program, GL_OBJECT_LINK_STATUS_ARB, &linked);

		if(!linked) {
			char errorString[4096];
			glGetInfoLogARB(shader->program, sizeof(errorString), NULL, errorString);
			Error::showAndExit("GLShader::create() error: shader file '" + path + "' shader linking error: " + String(errorString));
		}
		return shader;
	}

	//---------------------------------------------------------------------------
	//Desc:    GLShader desrtructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLShader::~GLShader() {
		if(vs) glDeleteObjectARB(vs);
		if(fs) glDeleteObjectARB(fs);
		glDeleteObjectARB(program);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets shader
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::set() {
		glUseProgramObjectARB(program);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets shader
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::unset() {
		glUseProgramObjectARB(NULL);
	}

	//---------------------------------------------------------------------------
	//Desc:    sends uniform param to shader
	//Params:  name - parameter name, value - param value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::sendMat4(const String &name, const Mat4 &value) {
		GLint param = glGetUniformLocationARB(program, name.c_str());
		glUniformMatrix4fvARB(param, 1, false, value);
	}

	//---------------------------------------------------------------------------
	//Desc:    sends uniform param to shader
	//Params:  name - parameter name, value - param value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::sendVec4(const String &name, const Vec4 &value) {
		GLint param = glGetUniformLocationARB(program, name.c_str());
		glUniform4fvARB(param, 1, value);
	}

	//---------------------------------------------------------------------------
	//Desc:    sends uniform param to shader
	//Params:  name - parameter name, value - param value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::sendVec3(const String &name, const Vec3 &value) {
		GLint param = glGetUniformLocationARB(program, name.c_str());
		glUniform3fvARB(param, 1, value);
	}

	//---------------------------------------------------------------------------
	//Desc:    sends uniform param to shader
	//Params:  name - parameter name, value - param value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::sendVec2(const String &name, const Vec2 &value) {
		GLint param = glGetUniformLocationARB(program, name.c_str());
		glUniform2fvARB(param, 1, value);
	}

	//---------------------------------------------------------------------------
	//Desc:    sends uniform param to shader
	//Params:  name - parameter name, value - param value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::sendFloat(const String &name, float value) {
		int param = glGetUniformLocationARB(program, name.c_str());
		glUniform1fARB(param, value);
	}

	//---------------------------------------------------------------------------
	//Desc:    sends uniform param to shader
	//Params:  name - parameter name, value - param value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLShader::sendInt(const String &name, int value) {
		int param = glGetUniformLocationARB(program, name.c_str());
		glUniform1iARB(param, value);
	}

}