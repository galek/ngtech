#include "EnginePrivate.h"

namespace NGTech {
	Engine engine;

	Engine* GetEngine() {
		return &engine;
	}
	FileSystem* GetVFS(){
		return GetEngine()->vfs;
	}
	IWindow* GetWindow(){
		return GetEngine()->iWindow;
	}
	CVARManager* GetCvars(){
		return GetEngine()->cvars;
	}
	IRender* GetRender(){
		return GetEngine()->iRender;
	}
	IAudio* GetAudio(){
		return GetEngine()->alSystem;
	}
	PhysSystem* GetPhysics(){
		return GetEngine()->physSystem;
	}
	Cache* GetCache(){
		return GetEngine()->cache;
	}
	GUI* GetGUI(){
		return GetEngine()->gui;
	}
	Scene* GetScene(){
		return GetEngine()->scene;
	}
	IGame* GetGame(){
		return GetEngine()->game;
	}
	IScriptInterp* GetScripting(){
		return GetEngine()->scripting;
	}
}