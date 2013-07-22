/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"

//**************************************
#include "MyGUI_OpenGLPlatform.h"
#include "MyGUI_Gui.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_Button.h"
#include "MyGUI_PointerManager.h"
#include "GUI.h"
#include "ILSystem.h"
#include "WindowSystem.h"
#include "Log.h"
#include "Cash.h"
#include "CVarManager.h"
#include "VFS.h"
//**************************************

namespace VEGA {

	void* GUI::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		// Load the image as a resource
		if (engine.vfs->isDataExist(_filename))
		{
			Common::IDataStream* stream = engine.vfs->getData(_filename);
			if (!stream)
				Warning("[GUI]Failed Loading GUI image!");
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
			ILenum imageType = ilTypeFromExt(filename.c_str());//ilDetermineType(filename.c_str());
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
			case IL_COLOUR_INDEX: convertToFormat = IL_RGB; break;
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
		else{
			Warning("[GUI]Failed Loading GUI image!File not found:%s" + _filename);
			return NULL;
		}
	}
	void GUI::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename){}

	GUI::GUI(CVARManager *_cvars)
		: mPlatform(nullptr),
		mGUI(nullptr),
		fpsLabel(nullptr),
		cvars(_cvars),
		mDebugShow(false)
	{
		mPlatform = new MyGUI::OpenGLPlatform();
		mGUI = new MyGUI::Gui();
	}
	void GUI::initialise()	{
		mPlatform->initialise(this);
		mPlatform->getDataManagerPtr()->addResourceLocation("data/gui/", true);
		resize(cvars->width, cvars->height);
		mGUI->initialise("MyGUI_Core.xml");
#if 1
		MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", 10, 10, 300, 26, MyGUI::Align::Default, "Main");
		button->setFontName("DejaVuSansFont.15");
		button->setCaption("Hello World!");
#endif
		showDebugInfo(cvars->showInfo);
	}
	//---------------------------------------------------------------------------
	//Desc:    GUI destructor
	//Params:  -
	//Returns: pointer to the GUI
	//---------------------------------------------------------------------------
	GUI::~GUI() {

	}
	//---------------------------------------------------------------------------
	//Desc:    draw all GUI widgets
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GUI::update() {
		engine.iRender->enable2d(false);
		engine.iRender->disableCulling();
		engine.iRender->enableBlending(GLSystem::ONE, GLSystem::ONE_MINUS_SRC_ALPHA);
		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		updateDebugInfo();//Nick:TODO:Вынести все в отдельный класс

		engine.iRender->disableBlending();
		engine.iRender->enableCulling();
		engine.iRender->enable3d();
	}
	//Nick:TODO:Вынести все в отдельный класс
	void GUI::createDebugInfo(){
		fpsLabel = mGUI->createWidget<MyGUI::TextBox>("TextBox", 100, 0, 180, 180, MyGUI::Align::Default, "Statistic", "InfoTextBox");
		fpsLabel->setTextColour(MyGUI::Colour::White);
		fpsLabel->setTextShadow(true);
		fpsLabel->setVisible(true);
		fpsLabel->setCaption("FPS: ");
	}
	void GUI::updateDebugInfo(){
		if (mDebugShow)
			if (engine.iWindow->getDTime() > EPSILON)
				fpsLabel->setCaption("FPS: " + StringHelper::fromInt(1000 / engine.iWindow->getDTime()));
	}
	void GUI::resize(int _width, int _height) {
		mPlatform->getRenderManagerPtr()->setViewSize(_width, _height);
	}

	void GUI::showDebugInfo(bool _show){
		if (_show)
			createDebugInfo();
		mDebugShow = _show;
	}
}