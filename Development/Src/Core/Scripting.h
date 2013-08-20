#pragma once

//***************************************************
#include "..\Common\IScripting.h"
#include "..\..\Externals\angelscriptsdk\angelscript\include\angelscript.h"
//***************************************************

namespace VEGA{
	
	class CoreScriptInterp:public IScriptInterp{
	protected:
		CoreScriptInterp();
		virtual ~CoreScriptInterp();
		virtual void initialise();
		virtual int runScript(const char* _name, bool debug=false);
		static  void scriptLogWrite(const asSMessageInfo *msg, void *param);
	private:
		void regCoreFunctions(asIScriptEngine*_engine);
		int  _executeScript(const char *scriptFile, bool debug);
		int  _compileScript(const char *scriptFile);
		void _saveJit();
	protected:
		asIScriptEngine*mASEngine;
		asCJITCompiler* jit;
	};
};