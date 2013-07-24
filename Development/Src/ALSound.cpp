/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//***************************************************************************
#include "ALSystem.h"
#include "Log.h"
#include "File.h" 
#include "Error.h"
#include "../Externals/libvorbis/include/vorbis/vorbisfile.h"
//***************************************************************************

namespace VEGA {

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