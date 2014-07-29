#pragma once
#include "DLLDef.h"
#include "../../Core/inc/Scripting.h"

namespace NGTech{
	class ENGINE_API EngineScriptInterp:public CoreScriptInterp
	{
	public:
		EngineScriptInterp();
		virtual void initialise();
	};
}