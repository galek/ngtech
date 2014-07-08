/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "../Common/EString.h"
//***************************************************************************

namespace NGTech {

//---------------------------------------------------------------------------
//Desc: Sound class 
//---------------------------------------------------------------------------
class ALSound {
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

};