/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    ALSystem.cpp
* Desc:    OpenAL sound system.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com> , Nick Galko <galek@nggames.com>
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
#include "Error.h"
//***************************************************************************

namespace NGTech {


	//---------------------------------------------------------------------------
	//Desc:    creates new ALSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ALSystem::ALSystem() : alDevice(nullptr),
		alContext(nullptr) {}

	//---------------------------------------------------------------------------
	//Desc:    creates new ALSystem
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSystem::initialise() {
		Log::writeHeader("-- ALSystem --");

		alDevice = alcOpenDevice(NULL);
		alContext = alcCreateContext(alDevice, NULL);
		alcMakeContextCurrent(alContext);

		Log::write("Vendor:     " + getVendor());
		Log::write("Renderer:   " + getRenderer());
		Log::write("Version:    " + getVersion());
		Log::write("Extensions: " + getExtensions());

		int error = alGetError();
		if (error != AL_NO_ERROR) {
			Error::showAndExit("ALSystem::create() error: error while creating");
			return;
		}
		alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	}


	//---------------------------------------------------------------------------
	//Desc:    ALSystem destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	ALSystem::~ALSystem() {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(alContext);
		alcCloseDevice(alDevice);
	}

	//---------------------------------------------------------------------------
	//Desc:    gets sound card vendor
	//Params:  -
	//Returns: vendor name
	//---------------------------------------------------------------------------
	String ALSystem::getVendor() {
		return (char *)alGetString(AL_VENDOR);
	}

	//---------------------------------------------------------------------------
	//Desc:    gets sound card renderer
	//Params:  -
	//Returns: renerer name
	//---------------------------------------------------------------------------
	String ALSystem::getRenderer() {
		return (char *)alGetString(AL_RENDERER);
	}

	//---------------------------------------------------------------------------
	//Desc:    gets OpenAL version
	//Params:  -
	//Returns: version number
	//---------------------------------------------------------------------------
	String ALSystem::getVersion() {
		return (char *)alGetString(AL_VERSION);
	}

	//---------------------------------------------------------------------------
	//Desc:    gets sound card extensions
	//Params:  -
	//Returns: extensions string
	//---------------------------------------------------------------------------
	String ALSystem::getExtensions() {
		return (char *)alGetString(AL_EXTENSIONS);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the listener orientation
	//Params:  position - position of the listener, dir - direction of the listener
	//Returns: -
	//---------------------------------------------------------------------------
	void ALSystem::setListener(const Vec3 &pos, const Vec3 &dir) {
		float orientation[] = { dir.x, dir.y, dir.z, 0, 1, 0 };
		alListenerfv(AL_POSITION, pos);
		alListenerfv(AL_ORIENTATION, orientation);
	}

}