#pragma once

//***************************************************************************
#include "StringHelper.h"
//***************************************************************************

namespace NGTech {

	/**
	Image class
	*/
	class I_ILImage {
	public:
		/**
		Image format
		*/
		enum Format {
			RGB = 0x1907,
			RGBA = 0x1908,
		};
	public:
		virtual ~I_ILImage(){}

		virtual void toNormalMap(int k) = 0;
		virtual void toGreyScale() = 0;

		unsigned char *getData() { return data; };

		int getWidth() { return width; };
		int getHeight() { return height; };
		int getDepth() { return depth; };
		int getBPP() { return bpp; };
		int getFormat() { return format; };

		bool is3D() { return depth > 1; };

	protected:
		int getSize() { return width * height * depth * bpp; };
	protected:
		int width, height, depth;
		int bpp, format;

		unsigned char *data;
	};
}