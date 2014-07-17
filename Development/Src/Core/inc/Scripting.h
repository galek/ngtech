#pragma once

//***************************************************
#include "coredll.h"
#include "..\..\Common\IScripting.h"
//***************************************************

namespace NGTech{
	
	class CORE_API CoreScriptInterp:public IScriptInterp{
	protected:
		CoreScriptInterp();
		virtual ~CoreScriptInterp();
		virtual void initialise();
		virtual int runScript(const char* _name);
	};
};