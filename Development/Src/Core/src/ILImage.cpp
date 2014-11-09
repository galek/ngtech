#include "CorePrivate.h"
//***************************************************************************
#include "ILImage.h"
#include "Log.h"
#include "FileHelper.h"
#include "MathLib.h"
#include "Error.h"
#include "VFS.h"
//***************************************************************************
#include "../../../Externals/ResIL/include/IL/il.h"
#include "../../../Externals/ResIL/include/IL/ilu.h"
//***************************************************************************

namespace NGTech {

	int BPP2Format(int bpp) {
		if (bpp == 3) return ILImage::RGB;
		if (bpp == 4) return ILImage::RGBA;
		return 0;
	}

	int Format2Bpp(int format) {
		if (format == ILImage::RGB) return 3;
		if (format == ILImage::RGBA) return 4;
		return 0;
	}

	ILImage *ILImage::createEmpty2d(int width, int height, int format) {
		ILImage *image = new ILImage();

		image->width = width;
		image->height = height;
		image->depth = 1;
		image->bpp = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		return image;
	}

	ILImage *ILImage::createNoise2d(int width, int height, int format) {
		ILImage *image = new ILImage();

		image->width = width;
		image->height = height;
		image->depth = 1;
		image->bpp = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		for (int i = 0; i < image->getSize(); i++) {
			image->data[i] = (ILubyte)rand() % 255;
		}

		return image;
	}

	ILImage *ILImage::createEmpty3d(int width, int height, int depth, int format) {
		ILImage *image = new ILImage();

		image->width = width;
		image->height = height;
		image->depth = depth;
		image->bpp = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		return image;
	}

	ILImage *ILImage::createNoise3d(int width, int height, int depth, int format) {
		ILImage *image = new ILImage();

		image->width = width;
		image->height = height;
		image->depth = depth;
		image->bpp = Format2Bpp(format);
		image->format = format;
		image->data = new ILubyte[image->getSize()];

		for (int i = 0; i < image->getSize(); i++) {
			image->data[i] = (ILubyte)rand() % 255;
		}

		return image;
	}

	ILImage *ILImage::create2d(const String &_path) {
		ILImage *image = new ILImage();
		if (GetVFS()->isDataExist(_path))
		{
			IDataStream* stream = GetVFS()->getData(_path);
			if (!stream){
				Warning("[GUI]Failed Loading GUI image!");
				return NULL;
			}
			size_t lumpSize = stream->size();
			void* lumpData = malloc(lumpSize);
			stream->read(lumpData, lumpSize);


#ifdef _UNICODE
			// Convert filename to a std::wstring
			std::wstring filename(_path.length(), L' ');
			std::copy(_path.begin(), _path.end(), _path.begin());
#else
			std::string filename = _path;
#endif

			// Try to determine the image type
			ILenum imageType = ilTypeFromExt(filename.c_str());
			if (imageType == IL_TYPE_UNKNOWN)
				imageType = ilDetermineTypeL(lumpData, lumpSize);

			// Try to load the image
			if (ilLoadL(imageType, lumpData, lumpSize) == IL_FALSE)
			{
				free(lumpData);
				return 0;
			}

			free(lumpData);

			// Retrieve image information
			image->width = ilGetInteger(IL_IMAGE_WIDTH);
			image->height = ilGetInteger(IL_IMAGE_HEIGHT);
			image->depth = 1;
			image->bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
			image->format = BPP2Format(image->bpp);
			ILenum type = ilGetInteger(IL_IMAGE_TYPE);


			// If the format is not supported, convert to a supported format
			// Also convert if the pixel type is not unsigned byte
			ILenum convertToFormat = image->format;

			switch (image->format)
			{
			case IL_COLOUR_INDEX: convertToFormat = IL_RGB; break;
			case IL_ALPHA: convertToFormat = IL_LUMINANCE_ALPHA; break;
			case IL_BGR: convertToFormat = IL_RGB; break;
			case IL_BGRA: convertToFormat = IL_RGBA; break;
			default: break;
			}

			if ((convertToFormat != image->format) || (type != IL_UNSIGNED_BYTE))
			{
				if (ilConvertImage(convertToFormat, IL_UNSIGNED_BYTE) == IL_FALSE)
				{
					return 0;
				}
			}

			// Copy the image data into some new memory
			ILint size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
			image->data = new unsigned char[size];
			ILubyte* data = ilGetData();
			memcpy(image->data, data, size);
			return image;
		}
		Warning("[GUI] Failed Loading GUI image!File not found:%s", _path);
		return NULL;
	}

	ILImage::~ILImage() {
		SAFE_DELETE(data);
	}

	void ILImage::toNormalMap(int k) {
		int w = width;
		int h = height;
		if (depth > 1)
			return;

		int byteCount = w*h;
		ILubyte* ndata = new ILubyte[byteCount];

		for (int i = 0; i < byteCount; i++) {
			ndata[i] = (data[i*bpp] + data[i*bpp + 1] + data[i*bpp + 2]) / 3.0;
		}

		float oneOver255 = 1.0 / 255.0;
		int   offs = 0;

		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				float c = ndata[i*h + j] * oneOver255;
				float cx = ndata[i*h + (j + 1) % w] * oneOver255;
				float cy = ndata[((i + 1) % h)*h + j] * oneOver255;

				float dx = (c - cx) * k;
				float dy = (c - cy) * k;

				float len = (float)sqrt(dx*dx + dy*dy + 1);

				float nx = dy / len;
				float ny = -dx / len;
				float nz = 1.0 / len;

				data[offs] = (ILubyte)(128 + 127 * nx);
				data[offs + 1] = (ILubyte)(128 + 127 * ny);
				data[offs + 2] = (ILubyte)(128 + 127 * nz);
				offs += bpp;
			}
		}
		delete[] ndata;
	}

	void ILImage::toGreyScale() {
		int w = width;
		int h = height;
		int d = depth;

		for (int i = 0; i < w*h*d; i++)
		{
			ILubyte color = (data[i*bpp]
				+ data[i*bpp + 1]
				+ data[i*bpp + 2]) / 3.0;

			data[i*bpp] = color;
			data[i*bpp + 1] = color;
			data[i*bpp + 2] = color;
		}
	}

}