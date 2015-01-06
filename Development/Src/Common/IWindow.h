/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    IWindow.h
* Desc:    Window interface.
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
#pragma once

//***************************************************
#ifdef WIN32
#include <windows.h>
#endif
#include <string>
//***************************************************

namespace NGTech {
	struct I_Window {
	public:
		/**
		*/
		virtual ~I_Window(){}
		/**
		*/
		virtual void initialise(int _hwnd) = 0;
		/**
		*/
		virtual void setTitle(const std::string &title) = 0;
		/**
		*/
		virtual void updateTimer() = 0;
		/**
		*/
		virtual void update() = 0;
		/**
		*/
		virtual int getWidth() = 0;
		/**
		*/
		virtual int getHeight() = 0;
		/**
		*/
		virtual int getDTime() = 0;
		/**
		*/
		virtual int getETime() = 0;
		/**
		*/
		virtual int getTime() = 0;
		/**
		*/
		virtual unsigned long GetTicks() = 0;
		/**
		*/
		virtual bool isMouseMoved() = 0;
		/**
		*/
		virtual int getMouseX() = 0;
		/**
		*/
		virtual int getMouseY() = 0;
		/**
		*/
		virtual int getMouseDX() = 0;
		/**
		*/
		virtual int getMouseDY() = 0;
		/**
		*/
		virtual void setMousePos(int x, int y) = 0;
		/**
		*/
		virtual void showCursor(bool show) = 0;
		/**
		*/
		virtual void toggleShowCursor() = 0;
		/**
		*/
		virtual bool isCursorVisible() = 0;
		/**
		*/
		virtual void grabMouse(bool grab) = 0;
		/**
		*/
		virtual void toggleGrabMouse() = 0;
		/**
		*/
		virtual bool isMouseGrabed() = 0;
		/**
		*/
		virtual bool isMouseButtonPressed(int mb) = 0;
		/**
		*/
		virtual bool wasMouseButtonPressed(int mb) = 0;
		/**
		*/
		virtual bool wasMouseButtonReleased(int mb) = 0;
		/**
		*/
		virtual bool isKeyPressed(const char* key) = 0;
		/**
		*/
		virtual bool isKeyDown(const char* key) = 0;
		/**
		*/
		virtual bool isKeyUp(const char* key) = 0;
		/**
		*/
		virtual void setKeyDown(int _value) = 0;
		/**
		*/
		virtual void setKeyUp(int _value) = 0;
		/**
		*/
		virtual void showOSCursor(int _value) = 0;
		/**
		*/
		virtual float getLastFPS() = 0;
		/**
		*/
		virtual const int Input_GetKeyValueByChar(const char*  _p) = 0;
		/**
		*/
		virtual const char* Input_GetKeyValueByInt(int _p) = 0;
		/**
		*/
		virtual void swapBuffers(){}
		/**
		*/
		virtual void DisableVSync(int){}
		/**
		*/
		inline virtual int divider() { return 1; }
	public:
		void* hRC;
		//For Native windows opengl
	public:
		int  bpp, zdepth;
		void* hInstance;
		void* hDC;
		int hWnd;
		unsigned int pixelFormat;
	};
}