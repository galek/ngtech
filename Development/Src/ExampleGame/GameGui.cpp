#include "stdafx.h"
#include "GameGUI.h"

#include "..\..\API\NGTechEngineAPI.h"
#include "MyGUI_Gui.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_Button.h"
using namespace MyGUI;
using namespace NGTech;

/*
*/
void GameGUIEvents::ExitEvent(MyGUI::Widget *)
{
	GetEngine()->quit();
}