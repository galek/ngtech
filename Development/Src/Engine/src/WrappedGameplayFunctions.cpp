#include "EnginePrivate.h"
#include "Scene.h"
#include "WrappedGameplayFunctions.h"

namespace NGTech
{
	/*
	*/
	ENGINE_API bool Script_SetActiveCameraFOV(float _fov)
	{
		auto cam = GetScene()->GetActiveCamera();
		if (!cam)
			return false;
		cam->setFOV(_fov);
		return true;
	}
}