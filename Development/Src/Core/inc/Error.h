/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#pragma once

//**************************************
#include "..\Common\EString.h"
//**************************************

namespace NGTech {

/**
Simple error viewing class
*/
	class CORE_API Error {
	public:
	/**
	Shows the error message box and exits
	\param text text to show
	*/
		static void showAndExit(const String &text);
	};

};