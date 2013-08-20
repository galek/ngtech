#include <assert.h>
#include "Log.h"
#include "IncludesAndLibs.h"
#include "scripting.h"
#include  <iostream>

#include "..\..\Externals\angelscriptsdk\angelscript\include\angelscript.h"
#include "..\..\Externals\angelscriptsdk\add_on\scriptbuilder\scriptbuilder.h"
#include "..\..\Externals\angelscriptsdk\add_on\scriptstdstring\scriptstdstring.h"
#include "..\..\Externals\angelscriptsdk\add_on\scriptarray\scriptarray.h"
#include "..\..\Externals\angelscriptsdk\angelscript\source\as_scriptengine.h"
#include "..\..\Externals\angelscriptsdk\add_on\debugger\debugger.h"
#include "..\..\Externals\angelscriptsdk\add_on\JitCompiler\as_jit.h"

namespace VEGA{

	/**
	*/
	CoreScriptInterp::CoreScriptInterp()
		:mASEngine(nullptr),
		jit(nullptr)	 {
			mASEngine=asCreateScriptEngine(ANGELSCRIPT_VERSION);
			if( mASEngine == 0 ){
				Error("[ScriptInterp] Failed to create script engine.",true);   
				return;
			}
			mASEngine->SetMessageCallback(asFUNCTION(scriptLogWrite),0,asCALL_CDECL);
			jit = new asCJITCompiler(0);//JIT TEST

			//Enable JIT helper instructions; without these,  the JIT will not be invoked
			mASEngine->SetEngineProperty(asEP_INCLUDE_JIT_INSTRUCTIONS, 1);//JIT TEST

			//Bind the JIT compiler to the engine
			mASEngine->SetJITCompiler(jit);//JIT TEST
	}

	/**
	*/
	void CoreScriptInterp::initialise(){
		RegisterStdString(mASEngine);
		RegisterScriptArray(mASEngine, false);
		RegisterStdStringUtils(mASEngine);
		this->regCoreFunctions(mASEngine);
	}

	/**
	*/
	void CoreScriptInterp::scriptLogWrite(const asSMessageInfo *msg, void *param){
		if( msg->type == asMSGTYPE_ERROR ) 
			Error(std::string(("%s (%d, %d) :  %s", msg->section, msg->row, msg->col,  msg->message)),false);
		else if( msg->type == asMSGTYPE_WARNING ) 
			Warning("%s (%d, %d) :  %s", msg->section, msg->row, msg->col,  msg->message);
		else if( msg->type == asMSGTYPE_INFORMATION ) 
			LogPrintf(std::string(("%s (%d, %d) :  %s", msg->section, msg->row, msg->col,  msg->message)));
	}

	/**
	*/
	int CoreScriptInterp::runScript(const char* _name, bool debug){
		int r=0;
		r=_compileScript(_name);
		_saveJit();
		if(r==0)
			r=_executeScript(_name,debug);
		return r;
	}

	/**
	*/
	CoreScriptInterp::~CoreScriptInterp()	{
		SAFE_DELETE(jit);//JIT TEST
		SAFE_RELEASE(mASEngine);
		//SAFE_DELETE(jit);//JIT TEST
	}

	/**
	*/
	int CoreScriptInterp::_compileScript(const char *scriptFile)
	{
		int r;
		Warning("_compileScript1");
		// We will only initialize the global variables once we're 
		// ready to execute, so disable the automatic initialization
		mASEngine->SetEngineProperty(asEP_INIT_GLOBAL_VARS_AFTER_BUILD, false);
		
		CScriptBuilder builder;

		r = builder.StartNewModule(mASEngine, "script");
		if( r < 0 ) return -1;

		r = builder.AddSectionFromFile(scriptFile);
		if( r < 0 ) return -1;
		r = builder.BuildModule();

		if( r < 0 )
		{
			Warning("[SCRIPT]Script '%s' failed to build",scriptFile);//TODO:line
			return -1;
		}

		Warning("[SCRIPT]Script '%s' successfully built",scriptFile);

		return 0;
	}

	/**
	*/
	int CoreScriptInterp::_executeScript(const char *scriptFile, bool debug)
	{
		CDebugger *dbg = 0;

		asIScriptModule *mod = mASEngine->GetModule("script");
		if( !mod ) return -1;

		// Find the main function
		asIScriptFunction *func = mod->GetFunctionByDecl("int main()");
		if( func == 0 )
		{
			// Try again with "void main()"
			func = mod->GetFunctionByDecl("void main()");
		}

		if( func == 0 )
		{                       
			Warning("[SCRIPT]Cannot find 'int main()' or 'void main()'");
			return -1;
		}

		// Set up a context to execute the script
		asIScriptContext *ctx = mASEngine->CreateContext();

		if( debug )
		{
			std::cout << "[SCRIPT]Debugging, waiting for commands. Type 'h' for help." << std::endl;

			// Create the debugger instance
			dbg = new CDebugger();

			// Allow the user to initialize the debugging before moving on
			dbg->TakeCommands(ctx);
		}

		if( dbg )
		{
			// Set the line callback for the debugging
			ctx->SetLineCallback(asMETHOD(CDebugger, LineCallback), dbg, asCALL_THISCALL);
		}

		// Once we have the main function, we first need to initialize the global variables
		// Pass our own context so the initialization of the global variables can be debugged too
		int r = mod->ResetGlobalVars(ctx);
		if( r < 0 )
		{
			Warning("[SCRIPT]Failed while initializing global variables");
			return -1;
		}

		// Execute the script
		ctx->Prepare(func);
		r = ctx->Execute();
		if( r != asEXECUTION_FINISHED )
		{
			if( r == asEXECUTION_EXCEPTION )
			{
				Warning("[SCRIPT]The script '%s' failed with an exception:in '%s',line %i",scriptFile,
					ctx->GetExceptionString(),ctx->GetLineNumber());
				r = -1;
			}
			else if( r == asEXECUTION_ABORTED )
			{
				Warning("[SCRIPT]The script '%s' was aborted on %i",
					scriptFile,ctx->GetLineNumber());
				r = -1;
			}
			else
			{
				Warning("[SCRIPT]The script %s terminated unexpectedly",scriptFile);
				r = -1;
			}
		}
		else
		{
			// Get the return value from the script
			if( func->GetReturnTypeId() == asTYPEID_INT32 )
				r = *(int*)ctx->GetAddressOfReturnValue();
			else r = 0;
		}
		ctx->Release();

		// Destroy debugger
		if( dbg )
			delete dbg;

		return r;
	}

	/**
	*/
	void CoreScriptInterp::_saveJit(){
		jit->finalizePages();
	}
}