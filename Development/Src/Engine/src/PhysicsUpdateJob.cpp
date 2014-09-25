#include "EnginePrivate.h"

#include "PhysicsUpdateJob.h"
#include "PhysSystem.h"

namespace NGTech{
	PhysicsUpdateJob::PhysicsUpdateJob() {}

	void PhysicsUpdateJob::process() {
		GetPhysics()->update();
	}
}