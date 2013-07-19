#pragma once

namespace VEGA {
	struct IGame
	{
		IGame();
		virtual void initialise()=0;
		virtual void Update()=0;
		
		
		void RunRenderCallback(){render_callback();};
		void RunEventsCallback(){events_callback();};

		typedef void (*EngineCallback)();
		//---------------------------------------------------------------------------
	//Desc:    sets render callback
	//Params:  callback - pointer to render function
	//Returns: -
	//---------------------------------------------------------------------------
	void SetRenderCallback(EngineCallback _rc){render_callback=_rc;rc = true;};
	//---------------------------------------------------------------------------
	//Desc:    sets events callback
	//Params:  callback - pointer to events function
	//Returns: -
	//---------------------------------------------------------------------------
	void SetEventsCallback(EngineCallback _ec){events_callback=_ec;ec = true;};
		
	bool rc, ec;
	EngineCallback render_callback;
	EngineCallback events_callback;
	};
}