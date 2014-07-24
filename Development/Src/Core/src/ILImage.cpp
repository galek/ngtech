/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "CorePrivate.h"
//***************************************************************************
#include "ILSystem.h"
#include "ILImage.h"
#include "Log.h"
#include "FileHelper.h"
#include "MathLib.h"
#include "Error.h"
//***************************************************************************

namespace NGTech {

	int BPP2Format(int bpp) {
		if(bpp == 3) return ILImage::RGB;
		if(bpp == 4) return ILImage::RGBA;
		return 0;
	}

	int Format2Bpp(int format) {
		if(format == ILImage::RGB) return 3;
		if(format == ILImage::RGBA) return 4;
		return 0;
	}

	ILImage *ILImage::createEmpty2d(int width, int height, int format) {
		ILImage *image = new ILImage();

		image->width  = width;
		image->height = height;
		image->depth  = 1;
		image->bpp    = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		return image;
	}

	ILImage *ILImage::createNoise2d(int width, int height, int format) {
		ILImage *image = new ILImage();

		image->width  = width;
		image->height = height;
		image->depth  = 1;
		image->bpp    = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		for(int i = 0; i < image->getSize(); i++) {
			image->data[i] = (ILubyte)rand()%255;
		}

		return image;
	}

	ILImage *ILImage::createEmpty3d(int width, int height, int depth, int format) {
		ILImage *image = new ILImage();

		image->width  = width;
		image->height = height;
		image->depth  = depth;
		image->bpp    = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		return image;
	}

	ILImage *ILImage::createNoise3d(int width, int height, int depth, int format) {
		ILImage *image = new ILImage();

		image->width  = width;
		image->height = height;
		image->depth  = depth;
		image->bpp    = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		for(int i = 0; i < image->getSize(); i++) {
			image->data[i] = (ILubyte)rand()%255;
		}

		return image;
	}

	ILImage *ILImage::create2d(const String &_path) {
		ILImage *image = new ILImage();
		VFile mFile(_path.c_str(), VFile::READ_BIN);
		String ext = mFile.GetFileExt();
		unsigned char *buf = (unsigned char*)mFile.LoadFile();
		unsigned int size = (unsigned int)mFile.Size();

		if (ext == "bmp")
			ilLoadL(IL_BMP, buf, size);
		if (ext == "tga")
			ilLoadL(IL_TGA, buf, size);
		if (ext == "jpg")
			ilLoadL(IL_JPG, buf, size);
		if(ext == "png")
			ilLoadL(IL_PNG, buf, size);
		if(ext == "dds")
			ilLoadL(IL_DDS, buf, size);
		if(ext == "gif")
			ilLoadL(IL_GIF, buf, size);

		int error = ilGetError();
		if(error != IL_NO_ERROR) {
			String errorStr = (const char *) iluErrorString(error);
			Log::write("ILImage::create2d() error: " + errorStr);
			return NULL;
		}

		image->width  = ilGetInteger(IL_IMAGE_WIDTH);
		image->height = ilGetInteger(IL_IMAGE_HEIGHT);
		image->depth  = 1;
		image->bpp    = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		image->format = BPP2Format(image->bpp);

		ILubyte *tempData = ilGetData();

		image->data = new ILubyte[image->getSize()];

		memcpy(image->data, tempData, image->getSize());
		return image;
	}

	ILImage::~ILImage() {
		free(data);
	}

	void ILImage::toNormalMap(int k) {
		int w = width;
		int h = height;
		if(depth > 1) 
			return;

		int byteCount = w*h;
		ILubyte* ndata = new ILubyte[byteCount];

		for(int i = 0; i < byteCount; i++) {
			ndata[i] = (data[ i*bpp ]  + data[i*bpp + 1] + data[i*bpp + 2]) / 3.0;
		}

		float oneOver255 = 1.0 / 255.0;
		int   offs = 0; 

		for(int i = 0; i < w; i++)
		{
			for(int j = 0; j < h; j++)
			{
				float c  = ndata[i*h + j        ] * oneOver255;
				float cx = ndata[i*h + (j+1)%w  ] * oneOver255;
				float cy = ndata[((i+1)%h)*h + j] * oneOver255;

				float dx = (c - cx) * k;
				float dy = (c - cy) * k;

				float len = (float) sqrt(dx*dx + dy*dy + 1);

				float nx = dy / len;
				float ny = -dx / len;
				float nz = 1.0 / len;

				data[ offs ]     = (ILubyte)(128 + 127*nx);
				data[ offs + 1 ] = (ILubyte)(128 + 127*ny);
				data[ offs + 2 ] = (ILubyte)(128 + 127*nz);
				offs += bpp;
			}
		}
		delete [] ndata;
	}

	void ILImage::toGreyScale() {
		int w = width;
		int h = height;
		int d = depth;

		for(int i = 0; i < w*h*d; i++)
		{
			ILubyte color = (data[ i*bpp ] 
			+ data[ i*bpp + 1 ] 
			+ data[ i*bpp + 2 ]) / 3.0;

			data[ i*bpp ] = color;
			data[ i*bpp + 1 ] = color;
			data[ i*bpp + 2 ] = color;
		}
	}

}