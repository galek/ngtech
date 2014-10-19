#include "EnginePrivate.h"

#include "AudioUpdateJob.h"
#include "Scene.h"

namespace NGTech{
	AudioUpdateJob::AudioUpdateJob() {}

	void AudioUpdateJob::process() {
		GetScene()->updateSound();
	}
}