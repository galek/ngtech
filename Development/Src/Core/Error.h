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

namespace VEGA {

	class Error {
	public:
		static void showAndExit(const String &text);
	};

};