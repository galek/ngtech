#pragma once

//***************************************************
#include "IncludesAndLibs.h"
#include "..\..\Common\IWindow.h"
#include "Config.h"
//***************************************************************************
#include "String.h"
//***************************************************

namespace NGTech {
#ifndef DROP_EDITOR
	struct CVARManager;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main window and input system. Created one time
	//---------------------------------------------------------------------------
	struct WindowSystem :public I_Window {
		/**
		Mouse buttons enum
		*/
		enum MouseButton {
			MOUSE_LEFT_BUTTON = 0,
			MOUSE_RIGHT_BUTTON = 1,
			MOUSE_RIGHT_MIDLLE = 2//Кажется она
		};
		/**
		Keyboard keys enum
		*/
		// see http://www.mods.com.au/budapi_docs/Virtual%20Key%20Codes.htm
		// and Platform\src\glfw\win32_window.c
		enum Key {
			KEY_ESC = VK_ESCAPE,
			KEY_SPACE = VK_SPACE,
			KEY_MINUS = VK_OEM_MINUS,
			KEY_RETURN = VK_BACK,
			KEY_LEFT_BRACKET = 0x1A,
			KEY_RIGHT_BRACKET = 0x1B,
			KEY_EQUAL = 0x0D,
			KEY_BACKSLASH = 0xDC/*VK_BACKSLASH*/,
			KEY_SEMICOLON = 0x27/*VK_SEMICOLON*/,
			KEY_APOSTROPHE = 0x28/*VK_APOSTROPHE*/,
			KEY_GRAVE_ACCENT = 0x29,

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

			KEY_F1 = VK_F1, KEY_F2,
			KEY_F3, KEY_F4,
			KEY_F5, KEY_F6,
			KEY_F7, KEY_F8,
			KEY_F9, KEY_F10,
			KEY_F11, KEY_F12,
			KEY_F13, KEY_F14,
			KEY_F15, KEY_F16,
			KEY_F17, KEY_F18,
			KEY_F19, KEY_F20,
			KEY_F21, KEY_F22,
			KEY_F23, KEY_F24,
			KEY_F25, KEY_F26
		};
	public:
		/**
		*/
		explicit WindowSystem(CVARManager*_cvars);
		/**
		*/
		virtual ~WindowSystem();
		/**
		*/
		virtual void initialise(int = 0);
		/**
		*/
		virtual void setTitle(const String &title);
		/**
		*/
		virtual void updateTimer();
		/**
		*/
		virtual void update();
		/**
		*/
		virtual int getTime();
		/**
		*/
		virtual void setMousePos(int x, int y);
		/**
		*/
		virtual void showCursor(bool show);
		/**
		*/
		virtual void grabMouse(bool grab);
		/**
		*/
		virtual bool isMouseButtonPressed(int mb);
		/**
		*/
		virtual bool wasMouseButtonPressed(int mb);
		/**
		*/
		virtual bool wasMouseButtonReleased(int mb);
		/**
		*/
		virtual bool isKeyPressed(const char* key);
		/**
		*/
		virtual bool isKeyDown(const char* key);
		/**
		*/
		virtual bool isKeyUp(const char* key);
		/**
		*/
		virtual void showOSCursor(int _value);
		/**
		*/
		virtual float getLastFPS();
		/**
		*/
		virtual unsigned long GetTicks();
		/**
		*/
		virtual const int Input_GetKeyValueByChar(const char*  _p);
		/**
		*/
		virtual const char* Input_GetKeyValueByInt(int _p);
		/**
		*/
		ENGINE_INLINE virtual int getWidth() { return width; };
		/**
		*/
		ENGINE_INLINE virtual int getHeight() { return height; };
		/**
		*/
		ENGINE_INLINE virtual int getDTime() { return dTime; };
		/**
		*/
		ENGINE_INLINE virtual int getETime() { return eTime; };
		/**
		*/
		ENGINE_INLINE virtual bool isMouseMoved() { return mousing; };
		/**
		*/
		ENGINE_INLINE virtual int getMouseX() { return mouseX; };
		/**
		*/
		ENGINE_INLINE virtual int getMouseY() { return mouseY; };
		/**
		*/
		ENGINE_INLINE virtual int getMouseDX() { return mouseX - oldMouseX; };
		/**
		*/
		ENGINE_INLINE virtual int getMouseDY() { return mouseY - oldMouseY; };
		/**
		*/
		ENGINE_INLINE virtual void toggleShowCursor() { showCursor(!cursorVisible); };
		/**
		*/
		ENGINE_INLINE virtual bool isCursorVisible() { return cursorVisible; };
		/**
		*/
		ENGINE_INLINE virtual void toggleGrabMouse() { grabMouse(!mouseGrabed); };
		/**
		*/
		ENGINE_INLINE virtual bool isMouseGrabed() { return mouseGrabed; };
		/**
		*/
		virtual void ManageVSync(bool _v);

		/**
		*/
		virtual void setKeyDown(int _value);

		/**
		*/
		virtual void setKeyUp(int _value);
	private:
		/**
		*/
		virtual void _updateFPSCounter();
		/**
		*/
		void _GetMonitorResolution(int hwnd, int &monitor_width, int &monitor_height);
		/**
		*/
	private:
		bool isExternalHwnd;
		float fps;

		//int  bpp, zdepth;
		bool fullscreen, withoutBorder;

		int eTime, dTime;
		int mouseX, mouseY;
		bool cursorVisible;
	public:
		int width, height;
		int oldMouseX, oldMouseY;

		bool mouseGrabed;

		int mx, my;
		bool mousing;

		bool mouseButtons[3];
		bool oldMouseButtons[3];

		bool keys[315];
		bool oldKeys[315];
	};
#endif
}