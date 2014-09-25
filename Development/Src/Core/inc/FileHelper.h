#pragma once

//**************************************
#include "../../Common/StringHelper.h"
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: some important File functions
	//---------------------------------------------------------------------------
	struct CORE_API FileHelper {
		static void writeString(FILE *file, const String &text);
		static String cutFileExt(const String &fName);
		static String getFileExt(const String &fName);
		static bool fileExist(const String &path);
	};

};