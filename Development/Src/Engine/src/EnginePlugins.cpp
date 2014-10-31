/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    EnginePlugins.cpp
* Desc:    Engine plugins manager impl.
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

namespace NGTech
{
	//-----------------------------------------------------------------------------
	// For plugin system...
	typedef void(*DLL_START_PLUGIN)(void);
	typedef void(*DLL_STOP_PLUGIN)(void);
	//-----------------------------------------------------------------------------
	void EnginePlugins::LoadEngineModule(const char *modulename)
	{
		// Load plugin library
		DynLib* module = new DynLib(modulename);
		module->Load();
		// Store for later unload
		modules.push_back(module);

		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)module->GetSymbol("dllStartPlugin");

		if (!pFunc)
			Warning("Cannot find symbol dllStartPlugin in library %s", modulename);
		else
		{
			std::string str = "Found symbol dllStartPlugin in library ";
			str += modulename;
			Debug(str);
			pFunc();
		};
	};


}