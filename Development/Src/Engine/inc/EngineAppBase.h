/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    EngineAppBase.cpp
* Desc:    Engine entrypoint.
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
#pragma once

namespace NGTech {
	struct IGame;
	struct ICallback;

	struct ENGINE_API EngineAppBase {
		explicit EngineAppBase(int _hwnd, bool _isEditor, IGame*_game, ICallback* rc, ICallback* ev);
		explicit EngineAppBase(IGame*_game, ICallback* rc, ICallback* ev);
		/**
		*/
		void Update();
		void Run();
	};
}