#pragma once

namespace NGTech
{
	/**
	*/
	class ObjectMesh;
	/**
	*/
	ENGINE_API void API_SetActiveCameraFOV(float _fov);
	/**
	*/
	ENGINE_API void API_PauseEngine(bool _s);
	/**
	*/
	ENGINE_API ObjectMesh* API_LoadModel(const char*_p);
	/**
	*/
	ENGINE_API void API_LoadEngineFormat(const char*_p);
	/**
	*/
	ENGINE_API const char* API_Input_GetKeyValueByInt(int _p);
	/**
	*/
	ENGINE_API const int API_Input_GetKeyValueByChar(const char*_p);
}