#pragma once

#include "EngineThreads.h"

namespace NGTech
{
	class PhysicsUpdateJob : public EngineJobBase
	{
	public:
		PhysicsUpdateJob();

		virtual void process();
	};
}