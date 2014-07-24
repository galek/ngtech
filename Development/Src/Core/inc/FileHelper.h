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

	//---------------------------------------------------------------------------
	//Desc: some important File functions
	//---------------------------------------------------------------------------
	struct CORE_API FileHelper {
		static String readString(FILE *file);
		static void writeString(FILE *file, const String &text);
		static String cutFileExt(const String &fName);
		static String getFileExt(const String &fName);
		static bool fileExist(const String &path);
	};

};