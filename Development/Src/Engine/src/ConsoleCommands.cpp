/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    Console.cpp
* Desc:    Console impl.
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

//***************************************************************************
#include "EnginePrivate.h"
//***************************************************************************
#include "Console.h"
#include "WrappedScriptFunctions.h"

namespace NGTech
{
	//***************************************************************************
	// !@E_Pause command
	void E_Pause(const MyGUI::UString & _key, const MyGUI::UString & _value) {
		bool status = false;
		if (!_value.empty()){
			MyGUI::utility::parseComplex(_value, status);
			API_PauseEngine(status);
		}
	}
	// !@E_SetActiveCameraFOV command
	void E_SetActiveCameraFOV(const MyGUI::UString & _key, const MyGUI::UString & _value) {
		bool status = false;
		if (!_value.empty()){
			MyGUI::utility::parseComplex(_value, status);
			API_SetActiveCameraFOV(status);
		}
	}
	// !@E_Quit command
	void E_Quit(const MyGUI::UString & _key, const MyGUI::UString & _value) {
		GetEngine()->quit();
	}
	//***************************************************************************
	void Console::_registerConsoleCommands()
	{
		this->registerConsoleDelegate("pause", MyGUI::newDelegate(E_Pause));
		this->registerConsoleDelegate("fov", MyGUI::newDelegate(E_SetActiveCameraFOV));
		this->registerConsoleDelegate("quit", MyGUI::newDelegate(E_Quit));
	}
}