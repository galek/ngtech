#include "EnginePrivate.h"
#include "Src/Engine.h"

#include "Main.h"
#include "IGame.h"


using namespace VEGA;

class ExampleGame:public IGame{
	virtual void initialise(){	init(); };
	virtual void Update(){};
};
typedef void (*EngineCallback)();//��� ������ � ������
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
};




//��� ������ � ������
void EngineStart(IGame*_game,EngineCallback rc=nullptr,EngineCallback ev=nullptr){
	EngineAppBase(_game,rc,ev);
}

//-------------------------------------------------------------
int main(int argc, char **argv) {
	EngineStart(new ExampleGame,render,events);
	return 0;
}
