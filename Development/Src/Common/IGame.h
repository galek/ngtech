/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    IGame.h
* Desc:    Game interface.
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
#pragma once

namespace VEGA {

	struct ICallback{
		virtual void Body() = 0;
	};

	struct IGame
	{
	protected:
		IGame():rc(false), ec(false){}
	public:
		virtual void initialise() = 0;
		virtual void update() = 0;


		void runRenderCallback(){ render_callback->Body(); };
		void runEventsCallback(){ events_callback->Body(); };

		//---------------------------------------------------------------------------
		//Desc:    sets render callback
		//Params:  callback - pointer to render function
		//Returns: -
		//---------------------------------------------------------------------------
		void setRenderCallback(ICallback* _rc){ render_callback = _rc; rc = true; };
		//---------------------------------------------------------------------------
		//Desc:    sets events callback
		//Params:  callback - pointer to events function
		//Returns: -
		//---------------------------------------------------------------------------
		void setEventsCallback(ICallback* _ec){ events_callback = _ec; ec = true; };

		bool rc, ec;
		ICallback* render_callback;
		ICallback* events_callback;
	};
}