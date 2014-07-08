#pragma once

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "ALSound.h"
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Sound source class
	//---------------------------------------------------------------------------
	class ALSoundSource {
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