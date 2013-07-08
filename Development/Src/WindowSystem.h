/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//***************************************************
#include "IncludesAndLibs.h"
#include "..\Common\IWindow.h"
#include "Config.h"
//***************************************************************************
#include "String.h"
//***************************************************

namespace NGEngine {
struct CVARManager;
//---------------------------------------------------------------------------
//Desc: Engine`s main window and input system. Created one time
//---------------------------------------------------------------------------
class WindowSystem:public IWindow {
public:
	WindowSystem(CVARManager*_cvars);
	
	~WindowSystem();
	virtual void initialise();
	virtual void setTitle(const String &title);

	virtual void updateTimer();
	virtual void swapBuffers();

	virtual void update();

	/*int getWidth() { return width; };
	int getHeight() { return height; };
		
	int getDTime() { return dTime; };
	int getETime() { return eTime; };
	*/
	virtual int getTime();

	/*bool isMouseMoved() { return mousing; };
	int getMouseX() { return mouseX; };
	int getMouseY() { return mouseY; };
	int getMouseDX() { return mouseX - oldMouseX; };
	int getMouseDY() { return mouseY - oldMouseY; };
	*/
	virtual void setMousePos(int x, int y);
	
	virtual void showCursor(bool show);
	/*void toggleShowCursor() { showCursor(!cursorVisible); };
	bool isCursorVisible() { return cursorVisible; };
	*/
	virtual void grabMouse(bool grab);
	/*void toggleGrabMouse() { grabMouse(!mouseGrabed); };
	bool isMouseGrabed() { return mouseGrabed; };
*/
	virtual bool isMouseButtonPressed(MouseButton mb);
	virtual bool wasMouseButtonPressed(MouseButton mb);
	virtual bool wasMouseButtonReleased(MouseButton mb);
	
	virtual bool isKeyPressed(Key key);
	virtual bool wasKeyPressed(Key key);
	virtual bool wasKeyReleased(Key key);

private:
	HDC hDC;		
	HGLRC hRC;		
	HWND hWnd;		
	HINSTANCE hInstance;	
};

};



