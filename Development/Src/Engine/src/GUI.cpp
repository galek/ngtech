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
#include "WindowSystem.h"
#include "Log.h"
#include "Cache.h"
#include "CVarManager.h"
#include "VFS.h"
//**************************************

namespace NGTech {

	GUI::GUI(CVARManager *_cvars)
		: mPlatform(nullptr),
		mGUI(nullptr),
		fpsLabel(nullptr),
		cvars(_cvars),
		mDebugShow(false)
	{
		Debug("[GUI] Constructor");
		mPlatform = new MyGUI::OpenGLPlatform();
		mGUI = new MyGUI::Gui();
	}
	void GUI::initialise()	{
		Log::writeHeader("-- GUI --");
		mPlatform->initialise((MyGUI::OpenGLImageLoader*)GetRender()->GetGUIImageLoader());
		mPlatform->getDataManagerPtr()->addResourceLocation("../data/gui/", true);
		resize(cvars->r_width, cvars->r_height);
		mGUI->initialise("MyGUI_Core.xml");
		showDebugInfo(cvars->r_showInfo);
	}
	
	GUI::~GUI() {
		SAFE_DELETE(fpsLabel);
		SAFE_DELETE(mGUI);
	}
	
	void GUI::update() {
#pragma message("TODO:GUI:Разобраться с апдейтом GUI")
		GetRender()->enable2d(false);
		GetRender()->disableCulling();
		GetRender()->enableBlending(I_Render::ONE, I_Render::ONE_MINUS_SRC_ALPHA);
		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		updateDebugInfo();//Nick:TODO:Вынести все в отдельный класс

		GetRender()->disableBlending();
		GetRender()->enableCulling();
		GetRender()->enable3d();
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
			if (GetWindow()->getDTime() > EPSILON)
				fpsLabel->setCaption("FPS: " + StringHelper::fromInt(int(GetEngine()->GetLastFPS())));
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