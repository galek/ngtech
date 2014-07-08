/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    ALSoundSource.cpp
* Desc:    OpenAL sound impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/
#include "EnginePrivate.h"
//***************************************************************************
#include "ALSystem.h"
#include "Log.h"
//***************************************************************************

namespace VEGA {

	//---------------------------------------------------------------------------
	//Desc:    creates new ALSoundSource
	//Params:  sound - pointer to the ALSound used by the ALSoundSource
	//Returns: pointer to new ALSoundSource
	//---------------------------------------------------------------------------
	ALSoundSource *ALSoundSource::create(ALSound *sound) {
		ALSoundSource *source = new ALSoundSource();

		alGenSources(1, &source->alID);
		alSourcei(source->alID, AL_BUFFER, sound->buffID);
		alSourcef(source->alID, AL_MIN_GAIN, 0.0f);
		alSourcef(source->alID, AL_MAX_GAIN, 1.0f);

		return source;
	}

	//---------------------------------------------------------------------------
	//Desc:    ALSoundSource destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ALSoundSource::~ALSoundSource() {
		alDeleteSources(1, &alID);
	}

	//---------------------------------------------------------------------------
	//Desc:    plays the ALSoundSource
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::play() {
		alSourcePlay(alID);
	}

	//---------------------------------------------------------------------------
	//Desc:    stops the ALSoundSource playing
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::stop() {
		alSourceStop(alID);
	}

	//---------------------------------------------------------------------------
	//Desc:    pauses the ALSoundSource playing
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::pause() {
		alSourcePause(alID);
	}

	//---------------------------------------------------------------------------
	//Desc:    is the ALSoundSource playing
	//Params:  -
	//Returns: true if ALSoundSource is playing
	//---------------------------------------------------------------------------
	bool ALSoundSource::isPlaying() {
		ALint state; 
		alGetSourcei(alID, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets ALSoundSource looping on/off
	//Params:  loop - loop flag
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::setLooping(bool loop) {
		alSourcei(alID, AL_LOOPING, loop);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables ALSoundSource relative position on/off
	//Params:  relative - relPos flag
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::setRelative(bool relative) {
		alSourcei(alID, AL_SOURCE_RELATIVE, relative);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets ALSoundSource gain
	//Params:  gain - gain value
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::setGain(float gain) {
		alSourcef(alID, AL_GAIN, gain);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets ALSoundSource position
	//Params:  pos - new ALSoundSource position
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::setPosition(const Vec3 &position) {
		alSourcefv(alID, AL_POSITION, position);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets ALSoundSource rolloff factor
	//Params:  rolloffFactor - rolloff factor value
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::setRolloffFactor(float rolloffFactor) {
		alSourcef(alID, AL_ROLLOFF_FACTOR, rolloffFactor);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets ALSoundSource reference distance
	//Params:  referenceDisatance - reference distance value
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::setReferenceDistance(float referenceDistance) {
		alSourcef(alID, AL_REFERENCE_DISTANCE, referenceDistance);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets ALSoundSource max distance
	//Params:  maxDisatance - max distance value
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSoundSource::setMaxDistance(float maxDistance) {;
	alSourcef(alID, AL_MAX_DISTANCE, maxDistance);
	}

}