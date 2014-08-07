#pragma once

//**************************************
#include "..\Common\StringHelper.h"
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