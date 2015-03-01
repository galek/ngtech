#pragma once

//**************************************
#include "DLLDef.h"
#include "Font.h"
#include <vector>
//**************************************
namespace MyGUI{
	class OpenGLPlatform;
	class Gui;
	class TextBox;
}

namespace NGTech {
	class Material;
	//----------------------------------------------------------------------------
	//Desc: Gui struct. Created one time
	//---------------------------------------------------------------------------
	class ENGINE_API GUI {
	public:
		explicit GUI(CVARManager *_cvars);

		~GUI();
		
		void render();
		void initialise();
		void resize(int _width, int _height);
		void showDebugInfo(bool _show);
		ENGINE_INLINE MyGUI::Gui* getGUI(){ return mGUI; };
	private:
		GUI(){}
		GUI(const GUI&){}
		//DebugInfo
		void CreateDebugInfo();
		void UpdateDebugInfo();
	private:
		MyGUI::OpenGLPlatform* mPlatform;
		MyGUI::Gui*mGUI;
		CVARManager *cvars;
		//DebugInfo
		bool mDebugShow;
		MyGUI::TextBox *fpsLabel, *LightCountLabel, *FBOCalls;
		Material *guiMtr;
		bool mInited;
	};
}