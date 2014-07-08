/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    GUI.cpp
* Desc:    GUI impl.
* Version: 1.0
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
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
#include "Cache.h"
#include "CVarManager.h"
#include "VFS.h"
//**************************************

namespace NGTech {

	void* GUI::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		// Load the image as a resource
		if (GetEngine()->vfs->isDataExist(_filename))
		{
			Common::IDataStream* stream = GetEngine()->vfs->getData(_filename);
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
			Warning("[GUI] Failed Loading GUI image!File not found:%s", _filename);
			return NULL;
		}
	}
	void GUI::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename){}

	GUI::GUI(CVARManager *_cvars)
		: mPlatform(nullptr),
		mGUI(nullptr),
		fpsLabel(nullptr),
		GUIRenderMtr(nullptr),
		cvars(_cvars),
		mDebugShow(false)
	{
		Debug("[GUI] Constructor");
		mPlatform = new MyGUI::OpenGLPlatform();
		mGUI = new MyGUI::Gui();
	}
	void GUI::initialise()	{
		Log::writeHeader("-- GUI --");
		mPlatform->initialise(this);
		mPlatform->getDataManagerPtr()->addResourceLocation("../data/gui/", true);
		resize(cvars->r_width, cvars->r_height);
		mGUI->initialise("MyGUI_Core.xml");
		GUIRenderMtr = new Material("engine_materials/gui.mat");
		showDebugInfo(cvars->r_showInfo);
	}
	//---------------------------------------------------------------------------
	//Desc:    GUI destructor
	//Params:  -
	//Returns: pointer to the GUI
	//---------------------------------------------------------------------------
	GUI::~GUI() {
		SAFE_DELETE(fpsLabel);
		SAFE_DELETE(GUIRenderMtr);
		SAFE_DELETE(mGUI);
	}
	//---------------------------------------------------------------------------
	//Desc:    draw all GUI widgets
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GUI::update() {
#if 0
//		GUIRenderMtr->setPass("Ambient");
//		GUIRenderMtr->unsetPass();	
#endif				
		GetEngine()->iRender->enable2d(false);
		GetEngine()->iRender->disableCulling();
		GetEngine()->iRender->enableBlending(GLSystem::ONE, GLSystem::ONE_MINUS_SRC_ALPHA);
		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		updateDebugInfo();//Nick:TODO:Вынести все в отдельный класс

		GetEngine()->iRender->disableBlending();
		GetEngine()->iRender->enableCulling();
		GetEngine()->iRender->enable3d();
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
			if (GetEngine()->iWindow->getDTime() > EPSILON)
				fpsLabel->setCaption("FPS: " + StringHelper::fromInt(1000 / GetEngine()->iWindow->getDTime()));
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