#include "stdafx.h"
#include "..\Engine\inc\dlldef.h"
#include "..\Engine\inc\EngineAppBase.h"
#include "ExampleGame.h"



namespace NGTech {
	struct ICallback;
	GAME_API void EngineStart(IGame*_game = nullptr, ICallback *rc = nullptr, ICallback *ev = nullptr){
		EngineAppBase(_game, rc, ev);
	}
}

GAME_API void ExampleGameStart(){
	EngineStart(new ExampleGame(), new RenderCallback(), new EventsCallback());
}
