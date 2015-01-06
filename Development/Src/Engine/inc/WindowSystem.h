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
		enum Key {
			KEY_ESC = VK_ESCAPE,
			KEY_SPACE = VK_SPACE,
			KEY_RETURN = VK_BACK,
			KEY_EQUAL = VK_OEM_3,

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
		virtual void DisableVSync(int);

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
	private:
		bool isExternalHwnd;
		float fps;

		//int  bpp, zdepth;
		bool fullscreen;

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