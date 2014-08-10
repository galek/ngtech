#pragma once

//***************************************************************************
#include "../../Core/inc/IncludesAndLibs.h"
//***************************************************************************
#include "../../Common/StringHelper.h"
#include "DLLDef.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Sound class 
	//---------------------------------------------------------------------------
	class ENGINE_API ALSound {
	public:
		static ALSound *create(const String &path);
		~ALSound();

	private:
		ALenum format;
		short *samples;

		int rate;
		int size;

		ALuint buffID;

		friend class ALSoundSource;
	};
}