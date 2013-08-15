/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    EngineAPPBase.cpp
* Desc:    Engine Application Base  impl.
* Version: 1.0
* Author:  Nick Galko <galek@vegaengine.com>
*
* This file is part of the Vega engine (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/
#include "..\EnginePrivate.h"
#include "EngineAppBase.h"
#include "..\Common\IGame.h"

namespace VEGA {

	EngineAppBase::EngineAppBase(IGame*_game, ICallback* rc, ICallback* ev){
		GetEngine()->setGame(_game);
		GetEngine()->initialise();
		if (rc)
			GetEngine()->game->setRenderCallback(rc);
		if (ev)
			GetEngine()->game->setEventsCallback(ev);
		Update();
	}
	/**
	*/
	void EngineAppBase::Update(){
		GetEngine()->mainLoop();
	}

}