#pragma once

namespace NGTech
{
	struct DebugLayer
	{
		DebugLayer();
		ENGINE_INLINE int GetRenderChangesOfFrameBufferrPerFrame(){ return renderChangesOfFrameBufferr; }
		ENGINE_INLINE int GetFrameNumber(){ return frameNumber; }
		ENGINE_INLINE void SetRenderChangesOfFrameBufferrPerFrame(int _v){ renderChangesOfFrameBufferr = _v; }
		ENGINE_INLINE void SetFrameNumber(int _v){ frameNumber = _v; }
		void UpdateDebug(){}
		void CleanDebugInfo();
		void CleanRenderChangesOfFrameBufferr();
	protected:
		int renderChangesOfFrameBufferr, frameNumber;
	};
}