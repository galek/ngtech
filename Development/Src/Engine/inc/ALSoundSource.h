#pragma once

//***************************************************************************
#include "../../Core/inc/IncludesAndLibs.h"
//***************************************************************************
#include "ALSound.h"
#include "DLLDef.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Sound source class
	//---------------------------------------------------------------------------
	class ENGINE_API ALSoundSource {
	public:
		static ALSoundSource *create(ALSound *sound);
		~ALSoundSource();

		void play();
		void stop();
		void pause();
		bool isPlaying();

		void setLooping(bool loop);
		void setRelative(bool relative);
		void setGain(float gain);
		void setPosition(const Vec3 &position);
		void setRolloffFactor(float rolloffFactor);
		void setReferenceDistance(float referenceDistance);
		void setMaxDistance(float maxDistance);

	private:
		ALuint alID;
	};
}