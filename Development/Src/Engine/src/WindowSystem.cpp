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
#ifndef DROP_EDITOR
	/**
	параметры, относящиеся к расчету FPS
	*/
	static LARGE_INTEGER CounterFrequency;
	static LARGE_INTEGER FPSCount;
	WindowSystem*_gWindowSystem = nullptr;

	/**
	*/
	LRESULT	CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

	/**
	*/
	WindowSystem::WindowSystem(CVARManager*_cvars)
		:isExternalHwnd(false),
		fps(0.0f),
		mouseGrabed(false),
		mousing(false),
		cursorVisible(false),
		mouseX(0),
		mouseY(0),
		oldMouseX(0),
		oldMouseY(0),
		width(_cvars->r_width),
		height(_cvars->r_height),
		fullscreen(_cvars->r_fullscreen)
	{
		Log::writeHeader("-- WindowSystem --");

		this->hWnd = 0;
		this->bpp = _cvars->r_bpp;
		this->zdepth = _cvars->r_zdepth;

		for (int i = 0; i < 3; i++)
			this->mouseButtons[i] = false;


		for (int i = 0; i < 315; i++)
			this->keys[i] = false;


		this->mouseGrabed = false;
		//инициализация таймеров
		QueryPerformanceFrequency(&CounterFrequency);
		QueryPerformanceCounter(&FPSCount);

		_gWindowSystem = this;
	}

	/**
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
			wc.hInstance = (HINSTANCE)this->hInstance;
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

			if (!(this->hWnd = (int)CreateWindowEx(dwExStyle,			// Extended Style For The Window
				"OpenGL",							// Class Name
				"NG Engine",						// Window Title
				dwStyle |							// Defined Window Style
				WS_CLIPSIBLINGS |					// Required Window Style
				WS_CLIPCHILDREN,					// Required Window Style
				0, 0,								// Window Position
				windowRect.right - windowRect.left,	// Calculate Window Width
				windowRect.bottom - windowRect.top,	// Calculate Window Height
				NULL,								// No Parent Window
				NULL,								// No Menu
				(HINSTANCE)hInstance,							// Instance
				NULL)))								// Dont Pass Anything To WM_CREATE
			{
				Error::showAndExit("WindowSystem::initialise() error: window creation error");
				return;
			}
		}
		else
			this->hWnd = _hwnd;

		if (!isExternalHwnd) {
			ShowWindow((HWND)hWnd, SW_SHOW);
			SetForegroundWindow((HWND)hWnd);
			SetFocus((HWND)hWnd);
		}
	}

	/**
	*/
	WindowSystem::~WindowSystem() {
		if (fullscreen) {
			ShowCursor(TRUE);
		}

		if (hDC) {
			ReleaseDC((HWND)hWnd, (HDC)hDC);
			hDC = NULL;
		}

		if (hWnd) {
			DestroyWindow((HWND)hWnd);
			hWnd = NULL;
		}

		UnregisterClass("OpenGL", (HINSTANCE)hInstance);
		hInstance = NULL;
	}

	/**
	*/
	void WindowSystem::setTitle(const String &title) {
		SetWindowText((HWND)hWnd, title.c_str());
	}

	/**
	*/
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
			if (_gWindowSystem)
				_gWindowSystem->keys[wParam] = true;
			//Nick:TODO: сделай ввод

			return 0;
		}

		case WM_KEYUP:
		{
			if (_gWindowSystem)
				_gWindowSystem->keys[wParam] = false;
			//Nick:TODO: сделай ввод

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			if (_gWindowSystem){
				_gWindowSystem->mx = LOWORD(lParam);
				_gWindowSystem->my = HIWORD(lParam);
				_gWindowSystem->mousing = true;


				if (!_gWindowSystem->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMouseMove(_gWindowSystem->oldMouseX, _gWindowSystem->oldMouseY, 0);

				return 0;
			}
		}

		case WM_LBUTTONDOWN:
		{
			if (_gWindowSystem)
				_gWindowSystem->mouseButtons[0] = true;

			if (!_gWindowSystem->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMousePress(_gWindowSystem->oldMouseX, _gWindowSystem->oldMouseY, (MyGUI::MouseButton::Enum)0);

			return 0;
		}

		case WM_LBUTTONUP:
		{
			if (_gWindowSystem)
				_gWindowSystem->mouseButtons[0] = false;

			if (!_gWindowSystem->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMouseRelease(_gWindowSystem->oldMouseX, _gWindowSystem->oldMouseY, (MyGUI::MouseButton::Enum)0);

			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			if (_gWindowSystem)
				_gWindowSystem->mouseButtons[1] = true;

			if (!_gWindowSystem->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMousePress(_gWindowSystem->oldMouseX, _gWindowSystem->oldMouseY, (MyGUI::MouseButton::Enum)1);

			return 0;
		}

		case WM_RBUTTONUP:
		{
			if (_gWindowSystem)
				_gWindowSystem->mouseButtons[1] = false;

			if (!_gWindowSystem->mouseGrabed)
				MyGUI::InputManager::getInstancePtr()->injectMouseRelease(_gWindowSystem->oldMouseX, _gWindowSystem->oldMouseY, (MyGUI::MouseButton::Enum)1);

			return 0;
		}

		case WM_SIZE:
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);

			if (GetRender())
				GetRender()->reshape(w, h);
			if (_gWindowSystem){
				_gWindowSystem->width = w;
				_gWindowSystem->height = h;
				return 0;
			}
			if (GetGUI())
				GetGUI()->resize(w, h);
		}
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	/**
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

	/**
	*/
	bool WindowSystem::isKeyPressed(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return keys[key];
	}

	/**
	*/
	bool WindowSystem::isKeyDown(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return (keys[key] && !oldKeys[key]);
	}

	/**
	*/
	bool WindowSystem::isKeyUp(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return (!keys[key] && oldKeys[key]);
	}

	/**
	*/
	bool WindowSystem::isMouseButtonPressed(int mb) {
		return mouseButtons[mb];
	}

	/**
	*/
	bool WindowSystem::wasMouseButtonPressed(int mb) {
		return (mouseButtons[mb] && !oldMouseButtons[mb]);
	}

	/**
	*/
	bool WindowSystem::wasMouseButtonReleased(int mb) {
		return (!mouseButtons[mb] && oldMouseButtons[mb]);
	}

	/**
	*/
	void WindowSystem::showCursor(bool show) {
		MyGUI::PointerManager::getInstance().setVisible(show);
		cursorVisible = show;
	}

	/**
	*/
	void WindowSystem::setMousePos(int x, int y) {
		POINT pt;
		pt.x = x;
		pt.y = y;
		ClientToScreen((HWND)hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	/**
	*/
	void WindowSystem::grabMouse(bool grab) {
		mouseX = oldMouseX = width / 2;
		mouseY = oldMouseY = height / 2;

		if (grab) { setMousePos(width / 2, height / 2); }
		showCursor(!grab);
		mouseGrabed = grab;
	}

	/**
	*/
	void WindowSystem::updateTimer() {
		int ticks = GetTickCount();
		dTime = ticks - eTime;
		eTime = ticks;
	}

	/**
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

	/**
	*/
	int WindowSystem::getTime() {
		return GetTickCount();
	}

	/*
	*/
	void WindowSystem::showOSCursor(int _value){
		if ((_value > 1) || (_value < 0))
			Warning("{[WindowSystem]showOSCursor} is not available in native window system. Is valid values: 0(false),1(true)");
		::ShowCursor(_value);
	}

	/**
	*/
	float WindowSystem::getLastFPS()
	{
		return fps;
	}

	/**
	*/
	const int WindowSystem::Input_GetKeyValueByChar(const char*  _p)
	{
		if (stricmp(_p, "A") == 0)
			return KEY_A;
		else if (stricmp(_p, "B") == 0)
			return KEY_B;
		else if (stricmp(_p, "C") == 0)
			return KEY_C;
		else if (stricmp(_p, "D") == 0)
			return KEY_D;
		else if (stricmp(_p, "E") == 0)
			return KEY_E;
		else if (stricmp(_p, "F") == 0)
			return KEY_F;
		else if (stricmp(_p, "G") == 0)
			return KEY_G;
		else if (stricmp(_p, "H") == 0)
			return KEY_H;
		else if (stricmp(_p, "I") == 0)
			return KEY_I;
		else if (stricmp(_p, "J") == 0)
			return KEY_J;
		else if (stricmp(_p, "K") == 0)
			return KEY_K;
		else if (stricmp(_p, "L") == 0)
			return KEY_L;
		else if (stricmp(_p, "M") == 0)
			return KEY_M;
		else if (stricmp(_p, "N") == 0)
			return KEY_N;
		else if (stricmp(_p, "O") == 0)
			return KEY_O;
		else if (stricmp(_p, "P") == 0)
			return KEY_P;
		else if (stricmp(_p, "Q") == 0)
			return KEY_Q;
		else if (stricmp(_p, "R") == 0)
			return KEY_R;
		else if (stricmp(_p, "S") == 0)
			return KEY_S;
		else if (stricmp(_p, "T") == 0)
			return KEY_T;
		else if (stricmp(_p, "U") == 0)
			return KEY_U;
		else if (stricmp(_p, "V") == 0)
			return KEY_V;
		else if (stricmp(_p, "W") == 0)
			return KEY_W;
		else if (stricmp(_p, "X") == 0)
			return KEY_X;
		else if (stricmp(_p, "Y") == 0)
			return KEY_Y;
		else if (stricmp(_p, "Z") == 0)
			return KEY_Z;

		else if (_p[0] == '0')
			return KEY_0;
		else if (_p[0] == '1')
			return KEY_1;
		else if (_p[0] == '2')
			return KEY_2;
		else if (_p[0] == '3')
			return KEY_3;
		else if (_p[0] == '4')
			return KEY_4;
		else if (_p[0] == '5')
			return KEY_5;
		else if (_p[0] == '6')
			return KEY_6;
		else if (_p[0] == '7')
			return KEY_7;
		else if (_p[0] == '8')
			return KEY_8;
		else if (_p[0] == '9')
			return KEY_9;

		else if ((stricmp(_p, "ESC")) == 0)
			return KEY_ESC;
		else if ((stricmp(_p, "UP")) == 0)
			return KEY_UP;
		else if ((stricmp(_p, "DONW")) == 0)
			return KEY_DOWN;
		else if ((stricmp(_p, "LETF")) == 0)
			return KEY_LEFT;
		else if ((stricmp(_p, "RIGHT")) == 0)
			return KEY_RIGHT;

		return 0;
	}

	/**
	*/
	const char* WindowSystem::Input_GetKeyValueByInt(int _p)
	{
		if (_p == KEY_A)
			return "A";
		else if (_p == KEY_B)
			return "B";
		else if (_p == KEY_C)
			return "C";
		else if (_p == KEY_D)
			return "D";
		else if (_p == KEY_E)
			return "E";
		else if (_p == KEY_F)
			return "F";
		else if (_p == KEY_G)
			return "G";
		else if (_p == KEY_H)
			return "H";
		else if (_p == KEY_I)
			return "I";
		else if (_p == KEY_J)
			return "J";
		else if (_p == KEY_K)
			return "K";
		else if (_p == KEY_L)
			return "L";
		else if (_p == KEY_M)
			return "M";
		else if (_p == KEY_N)
			return "N";
		else if (_p == KEY_O)
			return "O";
		else if (_p == KEY_P)
			return "P";
		else if (_p == KEY_Q)
			return "Q";
		else if (_p == KEY_R)
			return "R";
		else if (_p == KEY_S)
			return "S";
		else if (_p == KEY_T)
			return "T";
		else if (_p == KEY_U)
			return "U";
		else if (_p == KEY_V)
			return "V";
		else if (_p == KEY_W)
			return "W";
		else if (_p == KEY_X)
			return "X";
		else if (_p == KEY_Y)
			return "Y";
		else if (_p == KEY_Z)
			return "Z";

		else if (_p == KEY_0)
			return "0";
		else if (_p == KEY_1)
			return "1";
		else if (_p == KEY_2)
			return "2";
		else if (_p == KEY_3)
			return "3";
		else if (_p == KEY_4)
			return "4";
		else if (_p == KEY_5)
			return "5";
		else if (_p == KEY_6)
			return "6";
		else if (_p == KEY_7)
			return "7";
		else if (_p == KEY_8)
			return "8";
		else if (_p == KEY_9)
			return "9";

		else if (_p == KEY_ESC)
			return "ESC";
		else if (_p == KEY_SPACE)
			return "SPACE";
		else if (_p == KEY_UP)
			return "UP";
		else if (_p == KEY_DOWN)
			return "DOWN";
		else if (_p == KEY_LEFT)
			return "LEFT";
		else if (_p == KEY_RIGHT)
			return "RIGHT";

		return "NULL";
	}
#endif
}