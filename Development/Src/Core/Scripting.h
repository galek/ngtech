#pragma once

//***************************************************
#include "..\Common\IScripting.h"
//***************************************************

namespace NGTech{
	
	class CoreScriptInterp:public IScriptInterp{
	protected:
		CoreScriptInterp();
		virtual ~CoreScriptInterp();
		virtual void initialise();
		virtual int runScript(const char* _name);
	};
};