#pragma once

namespace VEGA {

	struct ICallback{
		virtual void Body() = 0;
	};

	struct IGame
	{
		IGame();
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