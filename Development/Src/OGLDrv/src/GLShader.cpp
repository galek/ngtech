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

		shader->vs = NULL;
		shader->fs = NULL;
		shader->cs = NULL;
		shader->tcs = NULL;
		shader->tes = NULL;
		shader->program = NULL;

		if (shader->_createShader(path, defines, true))
			return shader;
		else
			return NULL;
	}

	GLShader::~GLShader() {
		if (vs) glDeleteProgram(vs);
		if (fs) glDeleteProgram(fs);
		if (gs) glDeleteProgram(gs);
		if (cs) glDeleteProgram(cs);
		if (tcs) glDeleteProgram(tcs);
		if (tes) glDeleteProgram(tes);
		glDeleteProgram(program);
		glDeleteProgramPipelines(1, &PipelineName);
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

		unsigned int Format = 0;
		int Size = 0;
		std::vector<unsigned char> Data;
		bool mBinary = false;

		if (loadBinary(_createShaderCacheDirectory(path), Format, Data, Size))
		{
			this->program = glCreateProgram();
			glProgramParameteri(this->program, GL_PROGRAM_SEPARABLE, GL_TRUE);
			glProgramParameteri(this->program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
			glProgramBinary(this->program, Format, &Data[0], Size);
			mBinary = true;
			return true;
		}

		else {
			VFile mFile(path.c_str(), VFile::READ_TEXT);
			String line, vsCode, fsCode, tcsCode, tesCode, gsCode;

			while (!mFile.EndOfFile()) {
				line = mFile.GetLine();

				//find GLSL vertex shader
				if (line == "[GLSL_VERTEX_SHADER]") {
					vsCode = "";
					while (!mFile.EndOfFile()) {
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
					while (!mFile.EndOfFile()) {
						line = mFile.GetLine();
						if (line == "[GLSL_VERTEX_SHADER]") break;
						else if (line == "[GLSL_COMPUTE_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_CONTROL_SHADER]") break;
						else if (line == "[GLSL_TESSELATION_EVAL_SHADER]") break;
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
					while (!mFile.EndOfFile()) {
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
					while (!mFile.EndOfFile()) {
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
					while (!mFile.EndOfFile()) {
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
						Warning("[%s] Error: shader file '%s' tcs compiling error: %s", __FUNCTION__, path, (errorString));
						return false;
					}
				}
			}

			//create
			this->program = glCreateProgram();
			if (this->vs) glAttachShader(this->program, this->vs);
			if (this->fs) glAttachShader(this->program, this->fs);
			if (this->gs) glAttachShader(this->program, this->tcs);
			if (this->tes) glAttachShader(this->program, this->tes);
			if (this->tcs) glAttachShader(this->program, this->tcs);

			int linked;

			//Not needed for binary
			if (!mBinary)
				glLinkProgram(this->program);

			glGetProgramiv(this->program, GL_LINK_STATUS, &linked);

			if (!linked) {
				char errorString[4096];
				glGetInfoLogARB(this->program, sizeof(errorString), NULL, errorString);
				Warning("[%s] Error: shader file '%s' tcs compiling error: %s", __FUNCTION__, path, (errorString));
				return false;
			}
			glGenProgramPipelines(1, &PipelineName);

			if (_save)
				return _saveCache(path);

			return true;
		}
		return false;
	}

	bool GLShader::_saveCache(const String &path)
	{
		Debug(__FUNCTION__);
		GLint Size(0);
		GLenum Format(0);

		glGetProgramiv(this->program, GL_PROGRAM_BINARY_LENGTH, &Size);
		std::vector<unsigned char> Data(Size);
		glGetProgramBinary(this->program, Size, nullptr, &Format, &Data[0]);
		saveBinary(_createShaderCacheDirectory(path), Format, Data, Size);

		return true;
	}

	const char* GLShader::_createShaderCacheDirectory(String _file)
	{
		const char* dir = "../userData/ShaderCache/";
		String Filename = dir + _file + ".bin";
		_mkdir(dir);
		return Filename.c_str();
	}
}