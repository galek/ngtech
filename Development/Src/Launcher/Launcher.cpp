#include "stdafx.h"
#include "../Core/inc/BuildDefines.h"
#include "../ExampleGame/DLLx.h"
#include "Launcher.h"


GAME_API void ExampleGameStart();

#ifndef __LINUX
#include <Windows.h>

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	ExampleGameStart();
	return 0;
}
#endif

//-------------------------------------------------------------
int main(int argc, char **argv) {
	ExampleGameStart();
	
	return 0;
}