#include "RenderPrivate.h"
//***************************************************************************
#include "VFS.h"
#include "GUIImpl.h"
#include "GLSystem.h"
//***************************************************************************
#include "../../../Externals/ResIL/include/IL/il.h"
#include "../../../Externals/ResIL/include/IL/ilu.h"
//***************************************************************************

namespace NGTech {
	/*
	*/
	GUIImageLoader::GUIImageLoader() :
		MyGUI::OpenGLImageLoader(){
		ilInit();
		iluInit();
	}
	/*
	*/
	void* GUIImageLoader::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		// Load the image as a resource
		if (GetVFS()->isDataExist(_filename))
		{
			IDataStream* stream = GetVFS()->getData(_filename);
			if (!stream){
				Warning("[GUI]Failed Loading GUI image!");
				return nullptr;
			}
			size_t lumpSize = stream->size();
			void* lumpData = malloc(lumpSize);
			stream->read(lumpData, lumpSize);


#ifdef _UNICODE
			// Convert filename to a std::wstring
			std::wstring filename(_filename.length(), L' ');
			std::copy(_filename.begin(), _filename.end(), filename.begin());
#else
			std::string filename = _filename;
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
			_width = ilGetInteger(IL_IMAGE_WIDTH);
			_height = ilGetInteger(IL_IMAGE_HEIGHT);
			ILenum format = ilGetInteger(IL_IMAGE_FORMAT);
			ILenum type = ilGetInteger(IL_IMAGE_TYPE);


			// If the format is not supported, convert to a supported format
			// Also convert if the pixel type is not unsigned byte
			ILenum convertToFormat = format;

			switch (format)
			{
			case IL_COLOUR_INDEX: convertToFormat = IL_RGB; break; //-V525
			case IL_ALPHA: convertToFormat = IL_LUMINANCE_ALPHA; break;
			case IL_BGR: convertToFormat = IL_RGB; break;
			case IL_BGRA: convertToFormat = IL_RGBA; break;
			default: break;
			}

			if ((convertToFormat != format) || (type != IL_UNSIGNED_BYTE))
			{
				if (ilConvertImage(convertToFormat, IL_UNSIGNED_BYTE) == IL_FALSE)
				{
					return 0;
				}
			}

			// Determine MyGUI pixel formats
			switch (format)
			{
			case IL_RGB: _format = MyGUI::PixelFormat::R8G8B8; break;
			case IL_RGBA: _format = MyGUI::PixelFormat::R8G8B8A8; break;
			case IL_LUMINANCE: _format = MyGUI::PixelFormat::L8; break;
			case IL_LUMINANCE_ALPHA: _format = MyGUI::PixelFormat::L8A8; break;
			default: _format = MyGUI::PixelFormat::Unknow; break;
			}

			// Copy the image data into some new memory
			ILint size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
			void* _data = malloc(size);
			ILubyte* data = ilGetData();
			memcpy(_data, data, size);
			return _data;
		}
		Warning("[GUI] Failed Loading GUI image!File not found:%s", _filename.c_str());
		return nullptr;
	}
	/*
	*/
	GUIImageLoader*GUI_GetImageLoader(){ return new GUIImageLoader(); }
	/*
	*/
	void* GLSystem::GetGUIImageLoader(){ return GUI_GetImageLoader(); }
}