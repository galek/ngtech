/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
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
//***************************************************************************
#include "EnginePrivate.h"
//***************************************************************************
#include "string.h"//memset for gcc
//***************************************************************************
#include "../../Externals/libvorbis/include/vorbis/vorbisfile.h"
//***************************************************************************
#include "ALSystem.h"
#include "Log.h"
#include "FileHelper.h" 
#include "Error.h"
//***************************************************************************

namespace NGTech {

	ALSound::ALSound(const String &_path) {
		String path = "sounds/" + _path;
		if (FileHelper::getFileExt(path) != "ogg") {
			Error::showAndExit("ALSound::create() error: sound file " + path + " is not a OGG file");
			return;
		}

		VFile file(path, VFile::READ_BIN);
		OggVorbis_File vf;
		memset(&vf, 0, sizeof(vf));

		if (ov_open(file.GetLowLevelFile(), &vf, NULL, 0) < 0)	{
			Error::showAndExit("ALSound::create() error: sound file " + path + " is not a valid OGG file");
			return;
		}

		vorbis_info *vi = ov_info(&vf, -1);

		int numSamples = (int)ov_pcm_total(&vf, -1);
		int numChannels = vi->channels;

		if (numChannels == 1)
			this->format = AL_FORMAT_MONO16;
		else
			this->format = AL_FORMAT_STEREO16;


		this->rate = vi->rate;
		this->size = numSamples * numChannels;

		this->samples = new short[this->size];
		this->size *= sizeof(short);

		int samplePos = 0;
		while (samplePos < this->size) {
			char *dest = (char *)this->samples + samplePos;

			int bitStream, readBytes = ov_read(&vf, dest, this->size - samplePos, 0, 2, 1, &bitStream);
			if (readBytes <= 0)
				break;
			samplePos += readBytes;
		}
		ov_clear(&vf);

		alGenBuffers(1, &this->buffID);
		alBufferData(this->buffID, this->format, this->samples, this->size, this->rate);

		int error = alGetError();
		if (error != AL_NO_ERROR) {
			Error::showAndExit("ALSound::create() error: sound file " + path + " could not be loaded");
			return;
		}
	}

	ALSound::~ALSound() {
		alDeleteBuffers(1, &buffID);
		free(samples);
	}

}