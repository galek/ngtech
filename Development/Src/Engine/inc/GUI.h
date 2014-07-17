#pragma once

//**************************************
#include "DLLDef.h"
#include "Font.h"
#include <vector>
//**************************************
#include "MyGUI_OpenGLPlatform.h"
namespace MyGUI{
	class OpenGLPlatform;
	class Gui;
}

namespace NGTech {
	//----------------------------------------------------------------------------
	//Desc: Gui struct. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API GUI : public MyGUI::OpenGLImageLoader {
	public:
		explicit GUI(CVARManager *_cvars);

		~GUI();
		
		void update();
		void initialise();
		void resize(int _width, int _height);
		void showDebugInfo(bool _show);
		ENGINE_INLINE MyGUI::Gui* getGUI(){ return mGUI; };
	private:
		virtual void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename);
		virtual void* loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename);
		//DebugInfo
		void createDebugInfo();
		void updateDebugInfo();
	private:
		class MyGUI::OpenGLPlatform* mPlatform;
		class MyGUI::Gui*mGUI;
		CVARManager *cvars;
		class Material *GUIRenderMtr;
		//DebugInfo
		bool mDebugShow;
		class MyGUI::TextBox *fpsLabel;
	};
}