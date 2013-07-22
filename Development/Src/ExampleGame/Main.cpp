#include "stdafx.h"
#include "..\Engine\dlldef.h"
#include "..\Engine\EngineAppBase.h"
#include "..\ExampleGame\ExampleGame.h"



namespace VEGA {
	struct ICallback;
	GAME_API void EngineStart(IGame*_game = nullptr, ICallback *rc = nullptr, ICallback *ev = nullptr){
		EngineAppBase(_game, rc, ev);
	}
}

GAME_API void ExampleGameStart(){
	EngineStart(new ExampleGame(), new RenderCallback(), new EventsCallback());
}
