/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/
#pragma once
 
//***************************************************************************
#include "Core/IncludesAndLibs.h"
//***************************************************************************
#include "Common/EString.h"
//***************************************************************************

namespace VEGA {

/**
Image class  
*/
class ILImage {
public:
	/**
	Image format
	*/
	enum Format {
		RGB = IL_RGB,
		RGBA = IL_RGBA,
	};

public:
	static ILImage *create2d(const String &path);
	static ILImage *createEmpty2d(int width, int height, int format);
	static ILImage *createNoise2d(int width, int height, int format);
	static ILImage *createEmpty3d(int width, int height, int depth, int format);
	static ILImage *createNoise3d(int width, int height, int depth, int format);
	
	~ILImage();

	void toNormalMap(int k);
	void toGreyScale();

	unsigned char *getData() { return data; };

	int getWidth() { return width; };
	int getHeight() { return height; };
	int getDepth() { return depth; };
	int getBPP() { return bpp; };
	int getFormat() { return format; };

	bool is3D() { return depth > 1; };

private:
	int width, height, depth;
	int bpp, format;
		
	ILubyte *data;
	
	int getSize() { return width * height * depth * bpp; };
};

};