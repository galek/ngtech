/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    dllmain.cpp
* Desc:    DLL Entrypoint impl.
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
#include "RenderPrivate.h"
#include "GLSystem.h"

extern "C"
{
	void __declspec(dllexport) dllStartPlugin(void) throw(){
		using namespace NGTech;
		Debug("Loading dllStartPlugin");
		auto core = GetCore();
		core->SetRender(new GLSystem(core));
	}
}