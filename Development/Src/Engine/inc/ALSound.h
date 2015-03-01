#pragma once

//***************************************************************************
#include "../../Core/inc/IncludesAndLibs.h"
//***************************************************************************
#include "DLLDef.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Sound class 
	//---------------------------------------------------------------------------
	class ENGINE_API ALSound {
	public:
		ALSound(const String &path);
		~ALSound();

	private:
		int format;
		int rate;
		int size;
		unsigned int buffID;
		short *samples;
		friend class ALSoundSource;
	};
}