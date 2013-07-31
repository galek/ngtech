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