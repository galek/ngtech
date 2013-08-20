#pragma once
#include "Engine/DLLDef.h"
#include "Core/Scripting.h"

namespace VEGA{
	class VEGA_API EngineScriptInterp:public CoreScriptInterp
	{
	public:
		EngineScriptInterp();
		virtual void initialise();
	private:
		void regEngineFunctions(asIScriptEngine*_engine);
	};
}