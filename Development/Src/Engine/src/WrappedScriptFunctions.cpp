#include "EnginePrivate.h"
#include "Scene.h"
#include "WrappedScriptFunctions.h"

namespace NGTech
{
	/*
	*/
	ENGINE_API void API_SetActiveCameraFOV(float _fov)
	{
		auto cam = GetScene()->GetActiveCamera();
		if (cam)
			cam->setFOV(_fov);
	}
	/*
	*/
	ENGINE_API void API_PauseEngine(bool _s)
	{
		GetEngine()->SetPause(_s);
	}
	/*
	*/
	ENGINE_API ObjectMesh* API_LoadModel(const char*_p)
	{
		auto ptr = new ObjectMesh(_p);
		ptr->setMaterial("*", "defmat.mat");
		ptr->setTransform(Mat4::translate(Vec3(0, 0, 0)));
		return ptr;
	}
	/*
	*/
	ENGINE_API void API_LoadEngineFormat(const char*_p){
		DebugM("API_LoadEngineFormat:File %s", _p, __FILE__, __LINE__);
		API_LoadModel(_p);
	}
}