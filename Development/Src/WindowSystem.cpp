/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//***************************************************
#include "..\Common\IRender.h"
#include "..\Common\IWindow.h"
#include "WindowSystem.h"
#include "Log.h"
#include "Config.h"
#include "Error.h"
#include "CvarManager.h"
#include "GUI.h"
#include "mygui.h"
//***************************************************
#ifdef WIN32
#include <windows.h>
#endif
//***************************************************

namespace VEGA {

	LRESULT	CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

/*
*/
	WindowSystem::WindowSystem(CVARManager*_cvars) {

		Log::writeHeader("-- WindowSystem --");

		//-----read-config-values-----------------------------------
		this->width = _cvars->r_width;
		this->height = _cvars->r_height;
		this->bpp = _cvars->r_bpp;
		this->zdepth = _cvars->r_zdepth;
		this->fullscreen = _cvars->r_fullscreen;
		showOSCursor(false);//Сразу скрываем курсор
	}
	void WindowSystem::initialise(){

		unsigned int pixelFormat;
		WNDCLASS wc;
		DWORD dwExStyle;
		DWORD dwStyle;
		RECT windowRect;

		windowRect.left = (long) 0;
		windowRect.right = (long) width;
		windowRect.top = (long) 0;
		windowRect.bottom = (long) height;

		this->hInstance = GetModuleHandle(NULL);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC) wndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = this->hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "OpenGL";

		if (!RegisterClass(&wc)) {
			Error::showAndExit("WindowSystem::initialise() error: failed to register the window class");
			return;
		}

		if (fullscreen) {
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = width;
			dmScreenSettings.dmPelsHeight = height;
			dmScreenSettings.dmBitsPerPel = bpp;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
				fullscreen = false;
			}
		}

		if (fullscreen) {
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
		} else {
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

		if (!(this->hWnd = CreateWindowEx(dwExStyle,			// Extended Style For The Window
			"OpenGL",							// Class Name
			"VEGA Engine",						// Window Title
			dwStyle |							// Defined Window Style
			WS_CLIPSIBLINGS |					// Required Window Style
			WS_CLIPCHILDREN,					// Required Window Style
			0, 0,								// Window Position
			windowRect.right - windowRect.left,	// Calculate Window Width
			windowRect.bottom - windowRect.top,	// Calculate Window Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			this->hInstance,							// Instance
			NULL)))								// Dont Pass Anything To WM_CREATE
		{
			Error::showAndExit("WindowSystem::initialise() error: window creation error");
			return;
		}

		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bpp,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			zdepth,										// Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		if (!(this->hDC = GetDC(this->hWnd)))	{
			Error::showAndExit("WindowSystem::initialise() error: can't create a GL device context");
			return;
		}

		if (!(pixelFormat = ChoosePixelFormat(this->hDC, &pfd))) {
			Error::showAndExit("WindowSystem::initialise() error: can't find a suitable pixel format");
			return;
		}

		if (!(SetPixelFormat(this->hDC, pixelFormat, &pfd)))	{
			Error::showAndExit("WindowSystem::initialise() error: can't set the pixel format");
			return;
		}

		if (!(this->hRC = wglCreateContext(this->hDC)))	{
			Error::showAndExit("WindowSystem::initialise() error: can't create a GL rendering context");
			return;
		}

		if (!wglMakeCurrent(this->hDC, this->hRC)) {
			Error::showAndExit("WindowSystem::initialise() error: can't activate the GL rendering context");
			return;
		}

		ShowWindow(this->hWnd, SW_SHOW);
		SetForegroundWindow(this->hWnd);
		SetFocus(this->hWnd);

		for (int i = 0; i < 3; i++)
			this->mouseButtons[i] = false;


		for (int i = 0; i < 315; i++)
			this->keys[i] = false;


		this->mouseGrabed = false;
	}


/*
*/
	WindowSystem::~WindowSystem() {
		if (fullscreen) {
			ShowCursor(TRUE);
		}

		if (hRC) {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hRC);
			hRC = NULL;
		}

		if (hDC) {
			ReleaseDC(hWnd, hDC);
			hDC = NULL;
		}

		if (hWnd) {
			DestroyWindow(hWnd);
			hWnd = NULL;
		}

		UnregisterClass("OpenGL", hInstance);
		hInstance = NULL;
	}

/*
*/
	void WindowSystem::setTitle(const String &title) {
		SetWindowText(hWnd, title.c_str());
	}


	LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg)
		{
		case WM_CLOSE:
			{
				//PostQuitMessage(0);	
				exit(0);
				return 0;
			}

		case WM_KEYDOWN:
			{
				if (GetEngine()->iWindow)
					GetEngine()->iWindow->keys[wParam] = true;
				//Nick:TODO: сделай ввод

				return 0;
			}

		case WM_KEYUP:
			{
				if (GetEngine()->iWindow)
					GetEngine()->iWindow->keys[wParam] = false;
				//Nick:TODO: сделай ввод

				return 0;
			}

		case WM_MOUSEMOVE:
			{
				if (GetEngine()->iWindow){
					GetEngine()->iWindow->mx = LOWORD(lParam);
					GetEngine()->iWindow->my = HIWORD(lParam);
					GetEngine()->iWindow->mousing = true;


					if (!GetEngine()->iWindow->mouseGrabed)
						MyGUI::InputManager::getInstancePtr()->injectMouseMove(GetEngine()->iWindow->oldMouseX, GetEngine()->iWindow->oldMouseY, 0);
			
					return 0;
				}
			}

		case WM_LBUTTONDOWN:
			{
				if (GetEngine()->iWindow)
					GetEngine()->iWindow->mouseButtons[0] = true; 

				if (!GetEngine()->iWindow->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMousePress(GetEngine()->iWindow->oldMouseX, GetEngine()->iWindow->oldMouseY, (MyGUI::MouseButton::Enum)0);
				
				return 0;
			}

		case WM_LBUTTONUP:
			{
				if (GetEngine()->iWindow)
					GetEngine()->iWindow->mouseButtons[0] = false;

				if (!GetEngine()->iWindow->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMouseRelease(GetEngine()->iWindow->oldMouseX, GetEngine()->iWindow->oldMouseY, (MyGUI::MouseButton::Enum)0);
				
				return 0;
			}

		case WM_RBUTTONDOWN:
			{
				if (GetEngine()->iWindow)
					GetEngine()->iWindow->mouseButtons[1] = true;

				if (!GetEngine()->iWindow->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMousePress(GetEngine()->iWindow->oldMouseX, GetEngine()->iWindow->oldMouseY, (MyGUI::MouseButton::Enum)1);
				
				return 0;
			}

		case WM_RBUTTONUP:
			{
				if (GetEngine()->iWindow)
					GetEngine()->iWindow->mouseButtons[1] = false;

				if (!GetEngine()->iWindow->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMouseRelease(GetEngine()->iWindow->oldMouseX, GetEngine()->iWindow->oldMouseY, (MyGUI::MouseButton::Enum)1);
					
				return 0;
			}

		case WM_SIZE:
			{
				int w = LOWORD(lParam);
				int h = HIWORD(lParam);

				if (GetEngine()->iRender)
					GetEngine()->iRender->reshape(w, h);
				if (GetEngine()->iWindow){
					GetEngine()->iWindow->width = w;
					GetEngine()->iWindow->height = h;
					return 0;
				}
				if (GetEngine()->gui)
					GetEngine()->gui->resize(w, h);
			}
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

/*
*/
	void WindowSystem::update() {
		updateTimer();

		memcpy(oldKeys, keys, sizeof(keys));
		memcpy(oldMouseButtons, mouseButtons, sizeof(mouseButtons));
		mousing = false;

		MSG	msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (mouseGrabed) {
			oldMouseX = mouseX;
			oldMouseY = mouseY;
			mouseX += mx - width / 2;
			mouseY += my - height / 2;
			setMousePos(width / 2, height / 2);
		}else {
			oldMouseX = mouseX;
			oldMouseY = mouseY;
			mouseX = mx;
			mouseY = my;
		}
	}

/*
*/
	bool WindowSystem::isKeyPressed(Key key) {
		return keys[key];
	}

/*
*/
	bool WindowSystem::isKeyDown(Key key) {
		return (keys[key] && !oldKeys[key]);
	}

/*
*/
	bool WindowSystem::isKeyUp(Key key) {
		return (!keys[key] && oldKeys[key]);
	}

/*
*/
	bool WindowSystem::isMouseButtonPressed(MouseButton mb) {
		return mouseButtons[mb];
	}

/*
*/
	bool WindowSystem::wasMouseButtonPressed(MouseButton mb) {
		return (mouseButtons[mb] && !oldMouseButtons[mb]);
	}

/*
*/
	bool WindowSystem::wasMouseButtonReleased(MouseButton mb) {
		return (!mouseButtons[mb] && oldMouseButtons[mb]);
	}

/*
*/
	void WindowSystem::showCursor(bool show) {
		MyGUI::PointerManager::getInstance().setVisible(show);
		cursorVisible = show;
	}

/*
*/
	void WindowSystem::setMousePos(int x, int y) {
		POINT pt;
		pt.x = x;
		pt.y = y;
		ClientToScreen(hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

/*
*/
	void WindowSystem::grabMouse(bool grab) {
		mouseX = oldMouseX = width / 2;
		mouseY = oldMouseY = height / 2;

		if(grab) { setMousePos(width/2, height/2); }
		showCursor(!grab);
		mouseGrabed = grab;
	}

/*
*/
	void WindowSystem::swapBuffers() {
		SwapBuffers(hDC);
	}

/*
*/
	void WindowSystem::updateTimer() {
		int ticks = GetTickCount();
		dTime = ticks - eTime;
		eTime = ticks;
	}

/*
*/
	int WindowSystem::getTime() {
		return GetTickCount();
	}

/*
*/
	void WindowSystem::showOSCursor(bool _value){
		::ShowCursor(_value);	}

}