#pragma once

//**************************************
#include <string>
//**************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: std::string
	//---------------------------------------------------------------------------
	typedef std::string String;

	//---------------------------------------------------------------------------
	//Desc: some important String functions
	//---------------------------------------------------------------------------
	struct StringHelper {
		static int getWordsNumber(const String &input);
		static String getWord(const String &input, unsigned int n);

		static int toInt(const String &input);
		static float toFloat(const String &input);
		static double toDouble(const String &input);

		static int getInt(const String &input, unsigned int n);
		static float getFloat(const String &input, unsigned int n);
		static double getDouble(const String &input, unsigned int n);

		static String toUpper(const String &input);

		static String fromInt(int i);
		static String fromDouble(double i);
		static String fromFloat(float i);
		static String fromBool(bool i);
	};

}