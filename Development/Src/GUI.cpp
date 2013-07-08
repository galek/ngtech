/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"

//**************************************
#include "Includes\MyGUI_3.2.0\Platforms\OpenGL\OpenGLPlatform\include\MyGUI_OpenGLPlatform.h"
#include "Includes\MyGUI_3.2.0\MyGUIEngine\include\MyGUI_Gui.h"
#include "Includes\MyGUI_3.2.0\MyGUIEngine\include\MyGUI_TextBox.h"
#include "Includes\MyGUI_3.2.0\MyGUIEngine\include\MyGUI_Button.h"
#include "Includes\MyGUI_3.2.0\MyGUIEngine\include\MyGUI_PointerManager.h"
#include "GUI.h"
#include "ILSystem.h"
#include "WindowSystem.h"
#include "Log.h"
#include <gdiplus.h>
#pragma comment(lib,"Gdiplus.lib")
#include "Cash.h"
//**************************************

namespace NGEngine {

	GUI *gui;

	//**************************************************************************
	//GUI class
	//**************************************************************************
	//---------------------------------------------------------------------------
	//Desc:    creates new GUI
	//Params:  -
	//Returns: pointer to new GUI
	//---------------------------------------------------------------------------
	void convertRawData(Gdiplus::BitmapData* _out_data, void* _result, size_t _size, MyGUI::PixelFormat _format)
	{
		size_t num = 0;

		if (_format == MyGUI::PixelFormat::L8)
			num = 1;
		if (_format == MyGUI::PixelFormat::L8A8)
			num = 2;
		if (_format == MyGUI::PixelFormat::R8G8B8)
			num = 3;
		else if (_format == MyGUI::PixelFormat::R8G8B8A8)
			num = 4;
		else
			return;

		unsigned char* ptr_source = (unsigned char*)_out_data->Scan0;
		unsigned char* ptr_dest = (unsigned char*)_result;

		size_t stride_source = _out_data->Stride;
		size_t stride_dest = _out_data->Width * num;

		if (stride_dest == stride_source)
		{
			memcpy(_result, _out_data->Scan0, _size);
		}
		else
		{
			for (unsigned int y = 0; y < _out_data->Height; ++y)
			{
				memcpy(ptr_dest, ptr_source, stride_dest);
				ptr_dest += stride_dest;
				ptr_source += stride_source;
			}
		}
	}

	void* GUI::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		std::string fullname=MyGUI::OpenGLDataManager::getInstance().getDataPath(_filename);
		LogPrintf("loadImage:"+fullname);//Nick:debug
		void* result = 0;

		Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(MyGUI::UString(fullname).asWStr_c_str());
		if (image)
		{
			_width = image->GetWidth();
			_height = image->GetHeight();
			Gdiplus::PixelFormat format = image->GetPixelFormat();

			if (format == PixelFormat24bppRGB){
				_format = MyGUI::PixelFormat::R8G8B8;
				Debug("RGB");
			}
			else if (format == PixelFormat32bppARGB){
				_format = MyGUI::PixelFormat::R8G8B8A8;
				Debug("RGBA");
			}
			else{
				_format = MyGUI::PixelFormat::Unknow;
				Debug("Unknow");
			}

			if (_format != MyGUI::PixelFormat::Unknow)
			{
				Gdiplus::Rect rect(0, 0, _width, _height);
				Gdiplus::BitmapData out_data;
				image->LockBits(&rect, Gdiplus::ImageLockModeRead, format, &out_data);

				size_t size = out_data.Height * out_data.Stride;
				result = new unsigned char[size];

				convertRawData(&out_data, result, size, _format);

				image->UnlockBits(&out_data);
			}

			delete image;
		}else
			Debug("Failed Loading Image");

		return result;
	}
	void GUI::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename){
		Log::write("saveImage");//Nick:debug
		Gdiplus::PixelFormat format;
		int bpp;

		if (_format == MyGUI::PixelFormat::R8G8B8A8)
		{
			bpp = 4;
			format = PixelFormat32bppARGB;
		}
		else if (_format == MyGUI::PixelFormat::R8G8B8)
		{
			bpp = 3;
			format = PixelFormat24bppRGB;
		}
		else if (_format == MyGUI::PixelFormat::L8A8)
		{
			bpp = 2;
			format = PixelFormat16bppGrayScale;
		}
		else
		{
			MYGUI_LOG(Error, "Unsuitable texture format for saving.");
			return;
		}

		Gdiplus::Bitmap image(_width, _height, bpp * _width, format, (BYTE*)_texture);

		UINT num, size;
		Gdiplus::GetImageEncodersSize(&num, &size);

		Gdiplus::ImageCodecInfo* imageCodecInfo = (Gdiplus::ImageCodecInfo*)malloc(size);
		GetImageEncoders(num, size, imageCodecInfo);

		CLSID* pngClsid = NULL;
		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(imageCodecInfo[j].MimeType, L"image/png") == 0)
			{
				pngClsid = &imageCodecInfo[j].Clsid;
				break;
			}
		}

		if (pngClsid == NULL)
		{
			MYGUI_LOG(Error, "png codec not found");
			return;
		}

		HRESULT res = image.Save(MyGUI::UString(_filename).asWStr_c_str(), pngClsid, NULL);
		if (res != S_OK)
			MYGUI_LOG(Error, "Texture saving error. result =" << res);
	}
	
	GUI::GUI():mPlatform(nullptr),
		mGUI(nullptr),
		gdiplusToken(0)
	{		
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		mPlatform=new MyGUI::OpenGLPlatform();
		mGUI=new MyGUI::Gui();
	}
	void GUI::Initialise()	{	
		mPlatform->initialise(this);
		mPlatform->getDataManagerPtr()->addResourceLocation("data/gui/", true/*false*/);
		mPlatform->getDataManagerPtr()->addResourceLocation("data/guitest/", true/*false*/);
		mPlatform->getRenderManagerPtr()->setViewSize(1024.0f,768.0f/*1920,1080*/);
		mGUI->initialise("MyGUI_Core.xml");
#if 0
		//TEST
		MyGUI::TextBox*pMyGUIInfo = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>("TextBox", 100,100,180,180, MyGUI::Align::Default, "Statistic","InfoTextBox");
		pMyGUIInfo->setTextColour(MyGUI::Colour::White);
		pMyGUIInfo->setTextShadow(true);
		pMyGUIInfo->setVisible(true);
		pMyGUIInfo->setCaption("Testeeeeeeeeeeeeeeeeeeeeee");
		
		
		MyGUI::Gui::getInstance().load("MyButton.xml");
		MyGUI::Button* button =
			MyGUI::Gui::getInstance().createWidget<MyGUI::Button>(
			"MyButton",
			MyGUI::IntCoord(30, 30, 113, 27),
			MyGUI::Align::Default,
			"Main");
		button->setFontName("DejaVuSansFont.15");
		button->setTextAlign(MyGUI::Align::Center);
		button->setCaption("MyButton");
		button->setEnabled(true);

		MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", 10, 10, 300, 26, MyGUI::Align::Default, "Main");
		button->setFontName("DejaVuSansFont.15");
		button->setCaption("Hello World!");
#endif

		MyGUI::PointerManager::getInstance().setVisible(true);
	}
	//---------------------------------------------------------------------------
	//Desc:    GUI destructor
	//Params:  -
	//Returns: pointer to the GUI
	//---------------------------------------------------------------------------
	GUI::~GUI() {
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}



	//---------------------------------------------------------------------------
	//Desc:    draw all GUI widgets
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GUI::Update() {
		if(mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame(); 
	}

}

