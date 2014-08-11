#include "EnginePrivate.h"
#include "Scene.h"
#include "WrappedScriptFunctions.h"

namespace NGTech
{
	/*
	*/
	ENGINE_API bool API_SetActiveCameraFOV(float _fov)
	{
		auto cam = GetScene()->GetActiveCamera();
		if (!cam)
			return false;
		cam->setFOV(_fov);
		return true;
	}
	/*
	*/
	ENGINE_API void API_PauseEngine(bool _s)
	{
		GetEngine()->SetPause(_s);
	}
}