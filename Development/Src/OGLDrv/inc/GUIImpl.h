#pragma once

#include "RenderDLL.h"
#include "MyGUI_OpenGLPlatform.h"

namespace NGTech
{
	struct RENDER_API GUIImageLoader : public MyGUI::OpenGLImageLoader
	{
		GUIImageLoader();
		virtual ~GUIImageLoader(){}
		void*loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename);
		void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename){}
	};

	GUIImageLoader*GUI_GetImageLoader();
}