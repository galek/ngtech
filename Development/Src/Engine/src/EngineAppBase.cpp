/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    EngineAPPBase.cpp
* Desc:    Engine Application Base  impl.
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
#include "EnginePrivate.h"
#include "EngineAppBase.h"
#include "..\..\Common\IGame.h"

namespace NGTech {

	/**
	*/
	EngineAppBase::EngineAppBase(int _hwnd, bool _isEditor, IGame*_game, ICallback* rc, ICallback* ev){
		GetEngine()->RunEditor(_isEditor);
		GetEngine()->setGame(_game);
		GetEngine()->initialise(_hwnd);
		if (rc)
			GetGame()->setRenderCallback(rc);
		if (ev)
			GetGame()->setEventsCallback(ev);
		Update();
	}

	/**
	*/
	EngineAppBase::EngineAppBase(IGame*_game, ICallback* rc, ICallback* ev)
		:EngineAppBase(0,false, _game, rc, ev)
	{}

	/**
	*/
	void EngineAppBase::Update(){
		if (GetEngine()->isEditor())
			GetEngine()->editorLoop();
		else
			GetEngine()->mainLoop();
	}

	void EngineAppBase::Run(){
		GetEngine()->mainLoop();
	}

}