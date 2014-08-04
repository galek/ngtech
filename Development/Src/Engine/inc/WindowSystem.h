#pragma once

//***************************************************
#include "IncludesAndLibs.h"
#include "..\..\Common\IWindow.h"
#include "Config.h"
//***************************************************************************
#include "String.h"
//***************************************************

namespace NGTech {
	struct CVARManager;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main window and input system. Created one time
	//---------------------------------------------------------------------------
	class WindowSystem :public IWindow {
		bool isExternalHwnd;
	public:
		explicit WindowSystem(CVARManager*_cvars);

		~WindowSystem();
		virtual void initialise(int _hwnd = 0);
		virtual void setTitle(const String &title);

		virtual void updateTimer();

		virtual void update();

		virtual int getTime();

		virtual void setMousePos(int x, int y);

		virtual void showCursor(bool show);

		virtual void grabMouse(bool grab);

		virtual bool isMouseButtonPressed(MouseButton mb);
		virtual bool wasMouseButtonPressed(MouseButton mb);
		virtual bool wasMouseButtonReleased(MouseButton mb);

		virtual bool isKeyPressed(Key key);
		virtual bool isKeyDown(Key key);
		virtual bool isKeyUp(Key key);

		virtual void showOSCursor(bool _value);
	};
}