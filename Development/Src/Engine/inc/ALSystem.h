#pragma once

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "../Common/IAudio.h"
#include "ALSound.h"
#include "ALSoundSource.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Engine`s main sound system. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API ALSystem : public I_Audio {
	public:
		ALSystem();

		virtual ~ALSystem();

		virtual void initialise();
		virtual String getVendor();
		virtual String getRenderer();
		virtual String getVersion();
		virtual String getExtensions();

		virtual void setListener(const Vec3 &pos, const Vec3 &dir);

	private:
		ALCcontext *alContext;
		ALCdevice  *alDevice;
	};
}