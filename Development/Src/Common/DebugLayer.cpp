//***************************************************************************
#include "CommonPrivate.h"
//***************************************************************************
#include "DebugLayer.h"
//***************************************************************************

namespace NGTech
{
	DebugLayer::DebugLayer()
		:renderChangesOfFrameBufferr(-1),
		frameNumber(-1)
	{}

	int DebugLayer::GetRenderChangesOfFrameBufferrPerFrame()
	{
		return renderChangesOfFrameBufferr;
	}
}