/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    WindowSystem.cpp
* Desc:    Window system.
* Version: 1.0
* Author:  Alexander Tsyplyaev <tsyplyaev@gmail.com> , Nick Galko <galek@nggames.com>
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

namespace NGTech {
	//параметры, относящиеся к расчету FPS
	static LARGE_INTEGER CounterFrequency;
	static LARGE_INTEGER FPSCount;

	/*
	*/
	LRESULT	CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

	/*
	*/
	WindowSystem::WindowSystem(CVARManager*_cvars)
		: isExternalHwnd(false), fps(0.0f)
	{
		Log::writeHeader("-- WindowSystem --");

		//-----read-config-values-----------------------------------
		this->width = _cvars->r_width;
		this->height = _cvars->r_height;
		this->bpp = _cvars->r_bpp;
		this->zdepth = _cvars->r_zdepth;
		this->fullscreen = _cvars->r_fullscreen;
	}

	/*
	*/
	void WindowSystem::initialise(int _hwnd){
		isExternalHwnd = _hwnd != NULL;
		if (!isExternalHwnd)
			showOSCursor(false);

		if (_hwnd == 0)
		{
			WNDCLASS wc;
			DWORD dwExStyle;
			DWORD dwStyle;
			RECT windowRect;

			windowRect.left = (long)0;
			windowRect.right = (long)width;
			windowRect.top = (long)0;
			windowRect.bottom = (long)height;

			this->hInstance = GetModuleHandle(NULL);
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = (WNDPROC)wndProc;
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
			}
			else {
				dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
				dwStyle = WS_OVERLAPPEDWINDOW;
			}

			AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

			if (!(this->hWnd = CreateWindowEx(dwExStyle,			// Extended Style For The Window
				"OpenGL",							// Class Name
				"NGTech(http://nggames.com)",						// Window Title
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
		}
		else
			this->hWnd = (HWND)_hwnd;

		if (!isExternalHwnd) {
			ShowWindow(this->hWnd, SW_SHOW);
			SetForegroundWindow(this->hWnd);
			SetFocus(this->hWnd);
		}

		for (int i = 0; i < 3; i++)
			this->mouseButtons[i] = false;


		for (int i = 0; i < 315; i++)
			this->keys[i] = false;


		this->mouseGrabed = false;
		//инициализация таймеров
		QueryPerformanceFrequency(&CounterFrequency);
		QueryPerformanceCounter(&FPSCount);
	}

	/*
	*/
	WindowSystem::~WindowSystem() {
		if (fullscreen) {
			ShowCursor(TRUE);
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
			if (GetWindow())
				GetWindow()->keys[wParam] = true;
			//Nick:TODO: сделай ввод

			return 0;
		}

		case WM_KEYUP:
		{
			if (GetWindow())
				GetWindow()->keys[wParam] = false;
			//Nick:TODO: сделай ввод

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			if (GetWindow()){
				GetWindow()->mx = LOWORD(lParam);
				GetWindow()->my = HIWORD(lParam);
				GetWindow()->mousing = true;


				if (!GetWindow()->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMouseMove(GetWindow()->oldMouseX, GetWindow()->oldMouseY, 0);

				return 0;
			}
		}

		case WM_LBUTTONDOWN:
		{
			if (GetWindow())
				GetWindow()->mouseButtons[0] = true;

			if (!GetWindow()->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMousePress(GetWindow()->oldMouseX, GetWindow()->oldMouseY, (MyGUI::MouseButton::Enum)0);

			return 0;
		}

		case WM_LBUTTONUP:
		{
			if (GetWindow())
				GetWindow()->mouseButtons[0] = false;

			if (!GetWindow()->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMouseRelease(GetWindow()->oldMouseX, GetWindow()->oldMouseY, (MyGUI::MouseButton::Enum)0);

			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			if (GetWindow())
				GetWindow()->mouseButtons[1] = true;

			if (!GetWindow()->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMousePress(GetWindow()->oldMouseX, GetWindow()->oldMouseY, (MyGUI::MouseButton::Enum)1);

			return 0;
		}

		case WM_RBUTTONUP:
		{
			if (GetWindow())
				GetWindow()->mouseButtons[1] = false;

			if (!GetWindow()->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMouseRelease(GetWindow()->oldMouseX, GetWindow()->oldMouseY, (MyGUI::MouseButton::Enum)1);

			return 0;
		}

		case WM_SIZE:
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);

			if (GetRender())
				GetRender()->reshape(w, h);
			if (GetWindow()){
				GetWindow()->width = w;
				GetWindow()->height = h;
				return 0;
			}
			if (GetGUI())
				GetGUI()->resize(w, h);
		}
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	/*
	*/
	void WindowSystem::update() {
		_updateFPSCounter();
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
		}
		else {
			oldMouseX = mouseX;
			oldMouseY = mouseY;
			mouseX = mx;
			mouseY = my;
		}
	}

	/*
	*/
	bool WindowSystem::isKeyPressed(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return keys[key];
	}

	/*
	*/
	bool WindowSystem::isKeyDown(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return (keys[key] && !oldKeys[key]);
	}

	/*
	*/
	bool WindowSystem::isKeyUp(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
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

		if (grab) { setMousePos(width / 2, height / 2); }
		showCursor(!grab);
		mouseGrabed = grab;
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
	void WindowSystem::_updateFPSCounter()
	{
		//периодический расчет частоты смены кадров (каждые 50 кадров)
		static int iFrames = 0;    //счетчик кадров
		//fps = 0.0f;   //частота смены кадров (количество кадров в секунду)
		iFrames++;      //увеличение числа кадров на единицу при рендеринге
		if (iFrames == 50){
			float fTime;
			LARGE_INTEGER lCurrent;
			QueryPerformanceCounter(&lCurrent);   //получение текущего счетчика
			fTime = (float)(lCurrent.QuadPart - FPSCount.QuadPart) /
				(float)CounterFrequency.QuadPart;    //вычисление времени, за которое 50 раз перерисовалась форма
			fps = (float)iFrames / fTime;            //вычисление частоты смены кадров
			//обновление счетчика кадров и таймера
			iFrames = 0;
			QueryPerformanceCounter(&FPSCount);
		}
	}

	/*
	*/
	int WindowSystem::getTime() {
		return GetTickCount();
	}

	/*
	*/
	void WindowSystem::showOSCursor(bool _value){
		::ShowCursor(_value);
	}

	/*
	*/
	float WindowSystem::getLastFPS()
	{
		return fps;
	}
}