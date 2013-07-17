#include "EnginePrivate.h"
#include "Src/Engine.h"

#include "Main.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "IGame.h"


using namespace NGEngine;

class ExampleGame:public IGame{
	virtual void initialise(){	init(); };
	virtual void Update(){};
};
typedef void (*EngineCallback)();//Это оставь в хедере
struct EngineAppBase{
	EngineAppBase(IGame*_game,EngineCallback rc,EngineCallback ev){
		engine.SetGame(_game);
		engine.initialise();
		if(rc)
			engine.game->SetRenderCallback(rc);
		if(ev)
			engine.game->SetEventsCallback(ev);
		Update();
	}
	/**
	*/
	void Update(){
		engine.mainLoop();
	}
#ifdef _ENGINE_MT_
	void operator() (const tbb::blocked_range<bool>& range) const
	{			Debug("[threads]Called");		}
#endif
};




//Это оставь в хедере
void EngineStart(IGame*_game,EngineCallback rc=nullptr,EngineCallback ev=nullptr){
#ifdef _ENGINE_MT_
	tbb::parallel_for(tbb::blocked_range<bool>(1, 0), EngineAppBase(_game,rc,ev));
#else
	EngineAppBase(_game,rc,ev);
#endif
}

//-------------------------------------------------------------
int main(int argc, char **argv) {
	EngineStart(new ExampleGame,render,events);
	return 0;
}
