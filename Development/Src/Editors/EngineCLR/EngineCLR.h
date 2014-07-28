#pragma once
#include "..\..\Common\IWindow.h"
#include "..\..\Common\IRender.h"
#include "..\..\Core\inc\CoreManager.h"
#include "..\..\Engine\inc\dlldef.h"
#include "..\..\Engine\inc\Engine.h"
#include "..\..\Engine\inc\EngineAppBase.h"
#include "..\..\ExampleGame\ExampleGame.h"

using namespace System;
using namespace NGTech;

namespace EngineCLR {

	public ref class EngineCLR
	{
	public:
		EngineCLR(int _hwnd)
			:hwnd(_hwnd),
			mInited(false),
			engine(nullptr)
		{
		}

		~EngineCLR() {
			delete engine;
			engine = nullptr;
		}

		void EngineInit()
		{
			if (!engine)
				EngineStart(hwnd, new ExampleGame(), new RenderCallback(), new EventsCallback());
			MouseGrab(false);
		}
		bool isInited()
		{
			return mInited;
		}

		void Update() {
			if (engine) {
				engine->Update();
			}
		}

		void MouseGrab(bool _val)
		{
			if (GetWindow())
				GetWindow()->grabMouse(_val);
		}

		void Resize(int _w, int _h)	{
			GetRender()->reshape(_w, _h);
		}
	private:
		void EngineStart(int _hwnd, IGame*_game, ICallback *rc, ICallback *ev){
			if (!mInited)
			{
				engine = new EngineAppBase(_hwnd, true, _game, rc, ev);
				mInited = true;
			}
		}
	private:
		EngineAppBase* engine;
		int hwnd;
		bool mInited;
	};
}
