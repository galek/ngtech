//***************************************************************************
#include "CommonPrivate.h"
//***************************************************************************
#include "DebugLayer.h"
//***************************************************************************

namespace NGTech
{
	/**
	*/
	DebugLayer::DebugLayer()
		:renderChangesOfFrameBufferr(0),
		frameNumber(0)
	{}

	/**
	*/
	void DebugLayer::CleanDebugInfo() {
		renderChangesOfFrameBufferr = 0;
	}

	/**
	*/
	void DebugLayer::CleanRenderChangesOfFrameBufferr()
	{
		renderChangesOfFrameBufferr = 0;
	}
}