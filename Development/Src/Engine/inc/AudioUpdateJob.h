#pragma once

#include "EngineThreads.h"

namespace NGTech
{
	class AudioUpdateJob : public EngineJobBase
	{
	public:
		AudioUpdateJob();

		virtual void process();
	};
}