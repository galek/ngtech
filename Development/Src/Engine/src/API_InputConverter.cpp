#include "EnginePrivate.h"

#include "IWindow.h"

namespace NGTech {

	/**
	*/
	const int API_Input_GetKeyValueByChar(const char*  _p) {
		return GetWindow()->Input_GetKeyValueByChar(_p);
	}

	/**
	*/
	const char* API_Input_GetKeyValueByInt(int _p) {
		return GetWindow()->Input_GetKeyValueByInt(_p);
	}
}