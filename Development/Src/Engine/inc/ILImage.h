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
#include "../Common/IILImage.h"
//***************************************************************************

namespace NGTech {

	/**
	Image class
	*/
	class ILImage :public I_ILImage {
	public:
		static ILImage *create2d(const String &path);
		static ILImage *createEmpty2d(int width, int height, int format);
		static ILImage *createNoise2d(int width, int height, int format);
		static ILImage *createEmpty3d(int width, int height, int depth, int format);
		static ILImage *createNoise3d(int width, int height, int depth, int format);

		~ILImage();

		void toNormalMap(int k);
		void toGreyScale();
	};
}