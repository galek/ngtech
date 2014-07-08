/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    IScripting.h
* Desc:    Scripting interface.
* Version: 1.0
* Author:  Nick Galko <galek@vegaengine.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/
#pragma once

//Forward Declaration
class  asIScriptEngine;
struct asSMessageInfo;
class  asCJITCompiler;

namespace VEGA{
	struct IScriptInterp{
		IScriptInterp(){}
		virtual ~IScriptInterp(){}
		virtual void initialise()=0;
		virtual int runScript(const char* _name, bool debug=false)=0;
		void addInterp(IScriptInterp*_interp){}
	private:
		IScriptInterp(const IScriptInterp &){};
		IScriptInterp &operator = (const IScriptInterp &){};
	};
};