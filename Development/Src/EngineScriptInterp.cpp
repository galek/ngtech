#include <assert.h>
#include "EngineScriptInterp.h"
#include "log.h"

namespace VEGA{


	EngineScriptInterp::EngineScriptInterp(){}

	void EngineScriptInterp::initialise(){
		CoreScriptInterp::initialise();
	}

	void  EngineScriptInterp::regEngineFunctions(asIScriptEngine*_engine){
		Warning("EngineScriptInterp::regFunctions");
	}
}