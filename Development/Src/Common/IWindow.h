/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
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
	struct IWindow {
		//---------------------------------------------------------------------------
		//Desc: Mouse buttons enum
		//---------------------------------------------------------------------------
		enum MouseButton {
			MOUSE_LEFT_BUTTON = 0,
			MOUSE_RIGHT_BUTTON = 1,
		};

		//---------------------------------------------------------------------------
		//Desc: Keyboard keys enum
		//---------------------------------------------------------------------------
		enum Key {
			KEY_ESC = VK_ESCAPE,
			KEY_SPACE = VK_SPACE,

			KEY_UP = VK_UP,
			KEY_DOWN = VK_DOWN,
			KEY_LEFT = VK_LEFT,
			KEY_RIGHT = VK_RIGHT,

			KEY_0 = VK_NUMPAD0,
			KEY_1,
			KEY_2, KEY_3,
			KEY_4, KEY_5,
			KEY_6, KEY_7,
			KEY_8, KEY_9,

			KEY_A = 65,
			KEY_B,
			KEY_C, KEY_D,
			KEY_E, KEY_F,
			KEY_G, KEY_H,
			KEY_I, KEY_J,
			KEY_K, KEY_L,
			KEY_M, KEY_N,
			KEY_O, KEY_P,
			KEY_Q, KEY_R,
			KEY_S, KEY_T,
			KEY_U, KEY_V,
			KEY_W, KEY_X,
			KEY_Y, KEY_Z,
		};


		int width, height, bpp, zdepth;
		bool fullscreen;

		int eTime, dTime;
		int mouseX, mouseY;
		int oldMouseX, oldMouseY;

		bool cursorVisible;
		bool mouseGrabed;

		int mx, my;
		bool mousing;

		bool mouseButtons[3];
		bool oldMouseButtons[3];

		bool keys[315];
		bool oldKeys[315];
	public:
		virtual void initialise(int _hwnd) = 0;
		virtual void setTitle(const std::string &title) = 0;

		virtual void updateTimer() = 0;

		virtual void update() = 0;

		int getWidth() { return width; };
		int getHeight() { return height; };

		int getDTime() { return dTime; };
		int getETime() { return eTime; };
		virtual int getTime() = 0;

		bool isMouseMoved() { return mousing; };
		int getMouseX() { return mouseX; };
		int getMouseY() { return mouseY; };
		int getMouseDX() { return mouseX - oldMouseX; };
		int getMouseDY() { return mouseY - oldMouseY; };

		virtual void setMousePos(int x, int y) = 0;

		virtual void showCursor(bool show) = 0;
		void toggleShowCursor() { showCursor(!cursorVisible); };
		bool isCursorVisible() { return cursorVisible; };

		virtual void grabMouse(bool grab) = 0;
		void toggleGrabMouse() { grabMouse(!mouseGrabed); };
		bool isMouseGrabed() { return mouseGrabed; };

		virtual bool isMouseButtonPressed(MouseButton mb) = 0;
		virtual bool wasMouseButtonPressed(MouseButton mb) = 0;
		virtual bool wasMouseButtonReleased(MouseButton mb) = 0;

		virtual bool isKeyPressed(Key key) = 0;
		virtual bool isKeyDown(Key key) = 0;
		virtual bool isKeyUp(Key key) = 0;

		virtual void showOSCursor(bool _value) = 0;
	public:
		HDC hDC;
		HWND hWnd;
		HGLRC hRC;
		HINSTANCE hInstance;
		unsigned int pixelFormat;
	};
}