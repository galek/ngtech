#pragma once

//***************************************************
#include "coredll.h"
#include "..\..\Common\Types.h"
#include "..\..\Common\IScripting.h"
//***************************************************
struct lua_State;

namespace NGTech{
	
	class CORE_API CoreScriptInterp:public IScriptInterp{
	protected:
		CoreScriptInterp();
		virtual ~CoreScriptInterp();
		virtual void initialise();
		virtual int runScript(const char* _name);
		ENGINE_INLINE lua_State *GetLuaState(){ return mLuaState; }
	private:
		int doString(const char* code) const;
	private:
		void bindLogFunctions();
		void bindCoreFunctions();
		void bindMathFunctions();
	private:
		lua_State *mLuaState;
	};
};