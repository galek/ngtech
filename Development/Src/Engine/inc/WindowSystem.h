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

		virtual bool isKeyPressed(const char* key);
		virtual bool isKeyDown(const char* key);
		virtual bool isKeyUp(const char* key);

		virtual void showOSCursor(bool _value);

		virtual float getLastFPS();
	private:
		virtual void _updateFPSCounter();
	private:
		float fps;
	};
}