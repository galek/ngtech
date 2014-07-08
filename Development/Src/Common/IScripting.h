/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    IScripting.h
* Desc:    Scripting interface.
* Version: 1.0
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#pragma once

namespace NGTech{
	struct IScriptInterp{
		IScriptInterp(){}
		virtual ~IScriptInterp(){}
		virtual void initialise()=0;
		virtual int runScript(const char* _name)=0;
		void addInterp(IScriptInterp*_interp){}
	private:
		IScriptInterp(const IScriptInterp &){};
		IScriptInterp &operator = (const IScriptInterp &){};
	};
};