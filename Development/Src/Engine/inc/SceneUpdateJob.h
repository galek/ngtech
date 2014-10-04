#pragma once

#include "EngineThreads.h"

namespace NGTech
{
	class SceneUpdateJob : public EngineJobBase
	{
	public:
		SceneUpdateJob();

		virtual void process();
	};
}