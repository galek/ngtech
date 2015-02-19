#pragma once

namespace NGTech
{
	struct DebugLayer
	{
		DebugLayer();
		int GetRenderChangesOfFrameBufferrPerFrame();
		int renderChangesOfFrameBufferr, frameNumber;
	};
}