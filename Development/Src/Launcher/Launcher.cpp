// Launcher.cpp: определяет точку входа для приложения.
//
#include "stdafx.h"
#include "../Core/inc/BuildDefines.h"
#include "../ExampleGame/DLLx.h"
#include "Launcher.h"


GAME_API void ExampleGameStart();

#ifdef _WIN32
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
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