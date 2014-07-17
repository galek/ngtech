#pragma once

//***************************************************************************
#include "EString.h"
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
			RGB=0x1907,
			RGBA=0x1908,
		};
	public:
		~I_ILImage(){}

		void toNormalMap(int k){}
		void toGreyScale(){}

		unsigned char *getData() { return data; };

		int getWidth() { return width; };
		int getHeight() { return height; };
		int getDepth() { return depth; };
		int getBPP() { return bpp; };
		int getFormat() { return format; };

		bool is3D() { return depth > 1; };

	protected:
		int width, height, depth;
		int bpp, format;

		unsigned char *data;

		int getSize() { return width * height * depth * bpp; };
	};
}