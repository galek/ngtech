#include "EnginePrivate.h"

namespace NGTech {
	static Engine* engine = nullptr;

	Engine* GetEngine() {
		if (!engine) {
			engine = new Engine;
		}
		return engine;
	}

	void DestroyEngine() {
		delete engine;
		engine = nullptr;
	}
}