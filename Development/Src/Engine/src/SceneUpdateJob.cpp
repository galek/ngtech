#include "EnginePrivate.h"

#include "SceneUpdateJob.h"
#include "Scene.h"

namespace NGTech{
	SceneUpdateJob::SceneUpdateJob() {}

	void SceneUpdateJob::process() {
		GetScene()->updateSound();
	}
}