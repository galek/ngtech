#include "EnginePrivate.h"
#include "Src/Engine.h"

#include "Main.h"

using namespace NGEngine;
struct EngineApp{
	typedef void (*EngineCallback)();
	EngineApp(){
		engine.initialise();
	}
	/**
	*/
	void Update(){
		engine.mainLoop();
	}
	void SetRenderCallback(EngineCallback _callback){
		engine.renderCallback(_callback);
	}
	void SetEventsCallback(EngineCallback _callback){
		engine.eventsCallback(_callback);
	}
};

//-------------------------------------------------------------
int main(int argc, char **argv) {
	EngineApp*app=new EngineApp();
	init();
	app->SetRenderCallback(render);
	app->SetEventsCallback(events);

	app->Update();
	delete app;

	return 0;
}
