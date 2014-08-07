#pragma once

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "../../Common/StringHelper.h"
#include "../../Common/IILImage.h"
//***************************************************************************

namespace NGTech {

	/**
	Image class
	*/
	class CORE_API ILImage :public I_ILImage {
	public:
		static ILImage *create2d(const String &path);
		static ILImage *createEmpty2d(int width, int height, int format);
		static ILImage *createNoise2d(int width, int height, int format);
		static ILImage *createEmpty3d(int width, int height, int depth, int format);
		static ILImage *createNoise3d(int width, int height, int depth, int format);

		virtual ~ILImage();

		virtual void toNormalMap(int k);
		virtual void toGreyScale();
	};
}