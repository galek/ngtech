/* NG CONFIDENTIAL
* NGEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 NG Games Ltd.
* Author: Nick Galko
* E-mail: nick.galko@nggames.com
* All Rights Reserved.
*/
#pragma once

namespace NGTech
{
	class EnginePlugins
	{
	public:
		// Loads Engine Module from dll
		void LoadEngineModule(const char *modulename);
		// Plug-In dynamic libraries, platform-independent
		std::vector<class DynLib*> modules;
	};
}