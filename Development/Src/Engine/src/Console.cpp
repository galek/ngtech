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

#include <string>
#include <cctype>
#include <algorithm>
#include <iostream>

namespace NGTech
{

	Console::Console() : BaseLayout("Core/Console.layout")
	{
		assignWidget(mListHistory, "list_History");
		assignWidget(mComboCommand, "combo_Command");
		assignWidget(mButtonSubmit, "button_Submit");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr) window->eventWindowButtonPressed += newDelegate(this, &Console::notifyWindowButtonPressed);

		mStringCurrent = mMainWidget->getUserString("Current");
		mStringError = mMainWidget->getUserString("Error");
		mStringSuccess = mMainWidget->getUserString("Success");
		mStringUnknow = mMainWidget->getUserString("Unknown");
		mStringFormat = mMainWidget->getUserString("Format");

		mAutocomleted = false;

		mComboCommand->eventComboAccept += newDelegate(this, &Console::notifyComboAccept);
		mComboCommand->eventKeyButtonPressed += newDelegate(this, &Console::notifyButtonPressed);
		mButtonSubmit->eventMouseButtonClick += newDelegate(this, &Console::notifyMouseButtonClick);
		mListHistory->setOverflowToTheLeft(true);

		mMainWidget->setVisible(false);

		_registerConsoleCommands();
	}

	void Console::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button)
	{
		if (_button == "close")
		{
			mMainWidget->setVisible(false);

			GetWindow()->grabMouse(minputStatus);
		}
	}

	void Console::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		notifyComboAccept(mComboCommand, MyGUI::ITEM_NONE);
	}

	void Console::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		String command{ _sender->getOnlyText().asUTF8_c_str() };
		//������������ ��������� � ������ �������
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);

		if (command == "") return;

		String key = command;
		String value;

		size_t pos = command.find(' ');
		if (pos != String::npos)
		{
			key = command.substr(0, pos);
			value = command.substr(pos + 1);
		}

		MapDelegate::iterator iter = mDelegates.find(key);
		if (iter != mDelegates.end())
		{
			iter->second(key, value);
		}
		else
		{
			if (eventConsoleUnknowCommand.empty())
			{
				addToConsole(mStringUnknow + "'" + key + "'");
			}
			else
			{
				eventConsoleUnknowCommand(key, value);
			}
		}

		_sender->setCaption("");
	}


	void Console::notifyButtonPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
	{
		MyGUI::EditBox* edit = _sender->castType<MyGUI::EditBox>();
		size_t len = edit->getCaption().length();
		if ((_key == MyGUI::KeyCode::Backspace) && (len > 0) && (mAutocomleted))
		{
			edit->deleteTextSelection();
			len = edit->getCaption().length();
			edit->eraseText(len - 1);
		}

		String command = edit->getCaption();
		if (command.length() == 0)
			return;

		//������������ ��������� � ������ �������
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);
		edit->setCaption(command);


		for (MapDelegate::iterator iter = mDelegates.begin(); iter != mDelegates.end(); ++iter)
		{
			if (iter->first.find(command) == 0)
			{
				if (command == iter->first) break;
				edit->setCaption(iter->first);
				edit->setTextSelection(command.length(), iter->first.length());
				mAutocomleted = true;
				return;
			}
		}
		mAutocomleted = false;
	}

	void Console::addToConsole(const MyGUI::UString& _line)
	{
		if (mListHistory->getCaption().empty())
			mListHistory->addText(_line);
		else
			mListHistory->addText("\n" + _line);

		mListHistory->setTextSelection(mListHistory->getTextLength(), mListHistory->getTextLength());
	}

	void Console::clearConsole()
	{
		mListHistory->setCaption("");
	}

	void Console::registerConsoleDelegate(const MyGUI::UString& _command, CommandDelegate::IDelegate* _delegate)
	{
		mComboCommand->addItem(_command);
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			MYGUI_LOG(Warning, "console - command '" << _command << "' already exist");
		}
		mDelegates[_command] = _delegate;
	}

	void Console::unregisterConsoleDelegate(const MyGUI::UString& _command)
	{
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			mDelegates.erase(iter);
			for (size_t i = 0; i < mComboCommand->getItemCount(); ++i)
			{
				if (mComboCommand->getItemNameAt(i) == _command)
				{
					mComboCommand->removeItemAt(i);
					break;
				}
			}
		}
		else
			MYGUI_LOG(Warning, "console - command '" << _command << "' doesn't exist");
	}

	void Console::addToConsole(const MyGUI::UString& _reason, const MyGUI::UString& _key, const MyGUI::UString& _value)	{
		addToConsole(MyGUI::utility::toString(_reason, "'", _key, " ", _value, "'"));
	}

	const MyGUI::UString& Console::getConsoleStringCurrent() const 	{
		return mStringCurrent;
	}

	const MyGUI::UString& Console::getConsoleStringError() const 	{
		return mStringError;
	}

	const MyGUI::UString& Console::getConsoleStringSuccess() const 	{
		return mStringSuccess;
	}

	const MyGUI::UString& Console::getConsoleStringUnknow() const	{
		return mStringUnknow;
	}

	const MyGUI::UString& Console::getConsoleStringFormat() const	{
		return mStringFormat;
	}

	bool Console::getVisible()	{
		return mMainWidget->getVisible();
	}

	void Console::setVisible(bool _visible)	{
		mMainWidget->setVisible(_visible);
		minputStatus = GetWindow()->isMouseGrabed();
	}

}