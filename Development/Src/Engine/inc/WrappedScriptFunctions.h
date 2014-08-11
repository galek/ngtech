#pragma once

namespace NGTech
{
	/*
	*/
	ENGINE_API bool API_SetActiveCameraFOV(float _fov);
	/*
	*/
	ENGINE_API void API_PauseEngine(bool _s);
	/*
	*/
	ENGINE_API ObjectMesh* API_LoadModel(const char*_p);
	/*
	*/
	ENGINE_API void API_LoadEngineFormat(const char*_p);
}