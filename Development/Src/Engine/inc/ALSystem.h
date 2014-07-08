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
#include "../Common/IAudio.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Engine`s main sound system. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API ALSystem : public IAudio {
	public:
		ALSystem();

		~ALSystem();

		void initialise();
		String getVendor();
		String getRenderer();
		String getVersion();
		String getExtensions();

		void setListener(const Vec3 &pos, const Vec3 &dir);

	private:
		ALCcontext *alContext;
		ALCdevice  *alDevice;
	};
}