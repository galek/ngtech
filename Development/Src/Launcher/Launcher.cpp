// Launcher.cpp: определяет точку входа для приложения.
//
#include "..\Core\BuildDefines.h"
#include "stdafx.h"
#include "Launcher.h"


__declspec(dllimport) void ExampleGameStart();

#ifndef _ENGINE_DEBUG_
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	ExampleGameStart();
	return 0;
}
#else
//-------------------------------------------------------------
int main(int argc, char **argv) {
	ExampleGameStart();
	return 0;
}
#endif