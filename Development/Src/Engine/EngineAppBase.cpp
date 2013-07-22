#include "..\EnginePrivate.h"
#include "EngineAppBase.h"
#include "..\Common\IGame.h"

namespace VEGA {

	EngineAppBase::EngineAppBase(IGame*_game, ICallback* rc, ICallback* ev){
		engine.setGame(_game);
		engine.initialise();
		if (rc)
			engine.game->setRenderCallback(rc);
		if (ev)
			engine.game->setEventsCallback(ev);
		Update();
	}
	/**
	*/
	void EngineAppBase::Update(){
		engine.mainLoop();
	}

}