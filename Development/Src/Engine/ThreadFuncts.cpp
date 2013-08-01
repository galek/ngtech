
#include "..\EnginePrivate.h"
//***************************************************
#include "..\Engine.h"
#include "..\Core\CVARManager.h"
#include "..\WindowSystem.h"
#include "..\GLSystem.h"
#include "..\ILSystem.h"
#include "..\ALSystem.h"
#include "..\PhysSystem.h"
#include "..\Cash.h"
#include "..\Core\Log.h"
#include "..\Core\Config.h"
#include "..\LoadingScreen.h"
#include "..\Scene.h"
#include "..\Object.h"
#include "..\Terrain.h"
#include "..\Light.h"
#include "..\ParticleSystem.h"
#include "..\Camera.h"
#include "..\Font.h"
#include "..\GUI.h"
#include "..\Core\VFS.h"
#include "..\Common\IGame.h"

namespace VEGA {
	int ThreadUpdateCycle(Engine* engine){
		engine->mainLoop();
		return 1;
	}
	int ThreadUpdateCycle2(Engine* engine){
		if (engine->iWindow)
			engine->iWindow->update();

		if ((engine->physSystem) && (engine->iWindow))
			engine->physSystem->update(engine->iWindow->getDTime());

		if (engine->game)
			engine->game->runEventsCallback();

		if (engine->iRender)
			engine->iRender->clear(GLSystem::COLOR_BUFFER | GLSystem::DEPTH_BUFFER | GLSystem::STENCIL_BUFFER);

		if (engine->scene)
			engine->scene->Update();

		if (engine->game->rc)
			engine->game->runRenderCallback();

		if (engine->iRender)
			engine->iRender->flush();

		if (engine->iWindow)
			engine->iWindow->swapBuffers();

		if (engine->game)
			engine->game->update();
		return 1;
	}
};