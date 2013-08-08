#pragma once

//Forward Declaration
class  asIScriptEngine;
struct asSMessageInfo;
class  CScriptBuilder;

namespace VEGA{
	struct IScriptInterp{
	public:
		IScriptInterp();
		virtual void initialise()=0;
		virtual void runScript(const char* _name)=0;
		void addInterp(IScriptInterp*_interp){}
	protected:
		asIScriptEngine*mASEngine;
		CScriptBuilder* ASScriptB;
	};
};