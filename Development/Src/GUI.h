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


	//---------------------------------------------------------------------------
	//Desc: Gui struct. Created one time
	//---------------------------------------------------------------------------
	class VEGA_API GUI : public MyGUI::OpenGLImageLoader {
	public:
		GUI();

		~GUI();
		virtual void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename);
		virtual void* loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename);

		void update();
		void initialise();
	private:
		class MyGUI::OpenGLPlatform* mPlatform;
		class MyGUI::Gui*mGUI;

		//DebugInfo
	private://Nick:TODO:Вынести все в отдельный класс
		void createDebugInfo();
		void updateDebugInfo();
		class MyGUI::TextBox *fpsLabel;
	};

};