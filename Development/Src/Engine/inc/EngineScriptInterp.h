#pragma once
#include "DLLDef.h"
#include "..\Core\Scripting.h"

namespace NGTech{
	class VEGA_API EngineScriptInterp:public CoreScriptInterp
	{
	public:
		EngineScriptInterp();
		virtual void initialise();
	};
}