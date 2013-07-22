/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//**************************************
#include "GLSystem.h"
#include "Font.h"
#include <vector>
//**************************************
#include "MyGUI_OpenGLPlatform.h"
namespace MyGUI{
	class OpenGLPlatform;
	class Gui;
}

namespace VEGA {


	//----------------------------------------------------------------------------
	//Desc: Gui struct. Created one time
	//---------------------------------------------------------------------------
	class VEGA_API GUI : public MyGUI::OpenGLImageLoader {
	public:
		GUI(CVARManager *_cvars);

		~GUI();
		virtual void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename);
		virtual void* loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename);

		void update();
		void initialise();
		void resize(int _width, int _height);
		void showDebugInfo(bool _show);
	private:
		class MyGUI::OpenGLPlatform* mPlatform;
		class MyGUI::Gui*mGUI;
		CVARManager *cvars;
		//DebugInfo
	private://Nick:TODO:Вынести все в отдельный класс
		void createDebugInfo();
		void updateDebugInfo();
		bool mDebugShow;
		class MyGUI::TextBox *fpsLabel;
	};

};