/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    ALSound.cpp
* Desc:    OpenAL sound impl.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#include "EnginePrivate.h"
//***************************************************************************
#include "ALSystem.h"
#include "Log.h"
#include "File.h" 
#include "Error.h"
#include "../../Externals/libvorbis/include/vorbis/vorbisfile.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc:    creates ALSound from file
	//Params:  path - sound file path
	//Returns: -
	//---------------------------------------------------------------------------
	ALSound *ALSound::create(const String &path) {
		ALSound *sound = new ALSound();

		//#################################################
		//This code was written by Humus
		//#################################################
		if(FileHelper::getFileExt(path) != "ogg") {
			Error::showAndExit("ALSound::create() error: sound file " + path + " is not a OGG file");
			return NULL;
		} 

		FILE *file = fopen(path.c_str(), "rb");

		if(file == NULL) {
			Error::showAndExit("ALSound::create() error: sound file " + path + " not found");
			return NULL;
		}

		OggVorbis_File vf;
		memset(&vf, 0, sizeof(vf));

		if(ov_open(file, &vf, NULL, 0) < 0)	{
			fclose(file);
			Error::showAndExit("ALSound::create() error: sound file " + path + " is not a valid OGG file");
			return NULL;
		}

		vorbis_info *vi = ov_info(&vf, -1);

		int numSamples = (int)ov_pcm_total(&vf, -1);
		int numChannels = vi->channels;

		if(numChannels == 1) {
			sound->format = AL_FORMAT_MONO16;
		} else {
			sound->format = AL_FORMAT_STEREO16;
		}

		sound->rate = vi->rate;
		sound->size = numSamples * numChannels;

		sound->samples = new short[sound->size];
		sound->size *= sizeof(short);

		int samplePos = 0;
		while(samplePos < sound->size) {
			char *dest = (char *) sound->samples + samplePos;

			int bitStream, readBytes = ov_read(&vf, dest, sound->size - samplePos, 0, 2, 1, &bitStream);
			if(readBytes <= 0) 
				break;
			samplePos += readBytes;
		}
		ov_clear(&vf);
		//#################################################
		//#################################################

		alGenBuffers(1, &sound->buffID);
		alBufferData(sound->buffID, sound->format, sound->samples, sound->size, sound->rate);

		int error = alGetError();
		if(error != AL_NO_ERROR) {
			Error::showAndExit("ALSound::create() error: sound file " + path + " could not be loaded");
			return NULL;
		}

		return sound;
	}

	//---------------------------------------------------------------------------
	//Desc:    ALSound destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ALSound::~ALSound() {
		alDeleteBuffers(1, &buffID);
		free(samples);
	}

}