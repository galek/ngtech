#pragma once
#include "..\..\Engine\inc\dlldef.h"
#include "..\..\Engine\inc\EngineAppBase.h"

using namespace System;
using namespace NGTech;

namespace EngineCLR {

	public ref class EngineCLR
	{
		void EngineStart(int _hwnd,IGame*_game, ICallback *rc, ICallback *ev){
			EngineAppBase(_hwnd,_game, rc, ev);
		}
	};
}
