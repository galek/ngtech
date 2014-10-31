/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    EngineScriptInterp.cpp
* Desc:    Scripting impl.
* Version: 1.01
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
#include "EnginePrivate.h"
//***************************************************************************
#include <assert.h>
#include "EngineScriptInterp.h"
#include "log.h"

namespace NGTech{


	EngineScriptInterp::EngineScriptInterp(){}

	void EngineScriptInterp::initialise(){
		CoreScriptInterp::initialise();
	}
}