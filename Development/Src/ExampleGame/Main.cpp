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
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)
	//    _crtBreakAlloc = 931; //example break on alloc number 1027, change 
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	EngineStart(new ExampleGame(), new RenderCallback(), new EventsCallback());
}