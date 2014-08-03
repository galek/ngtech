//**************************************
#include "String.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
//**************************************

namespace NGTech {
	void replaceSpaces(String &s) {
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == ' ')
				s[i] = '_';
		}
	}

	void getFileName(String &s) {
		if (s.size() == 0) return;

		bool hasslash = false;
		for (int y = 0; y < s.size(); y++) {
			if (s[y] == '\\') hasslash = true;
		}

		if (!hasslash) return;

		int i = s.size();
		String buf;
		while ((s[i] != '\\') && (i > 0))
		{
			buf = s[i--] + buf;
		}
		s = buf;
		return;
	}

	void cutFileExt(String &s) {
		String out = _T("");
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '.') break;
			out = out + s[i];
		}
		s = out;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets words count. Words are separated by space
	//Params:  input - String containing words
	//Returns: number of words in String
	//---------------------------------------------------------------------------
	int StringHelper::getWordsNumber(const String &input) {
		int n = 0;
		size_t pos = 0;

		if (input[pos] == ' ') pos = input.find_first_not_of(' ', pos);
		while (pos < input.length()) {
			n++;
			pos = input.find_first_of(' ', pos);
			if (pos < input.length()) pos = input.find_first_not_of(' ', pos);
		}
		return n;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets word from String. Words are separated by space
	//Params:  input - String containing words, n - word number
	//Returns: word from the String
	//---------------------------------------------------------------------------
	String StringHelper::getWord(const String &input, unsigned int n) {
		String output;
		unsigned int k = 0;
		size_t pos = 0, last = 0;

		if ((input[pos] == ' ') || (input[pos] == '\x9')) {
			pos = output.find_first_not_of(_T(" \x9"), pos);
		}

		while (pos < input.length()) {
			k++;
			last = pos;
			pos = input.find_first_of(' ', pos);
			if (k == n) {
				output = input.substr(last, pos - last);
				return output;
			}
			if (pos < input.length()) {
				pos = input.find_first_not_of(' ', pos);
			}
		}
		return _T("");
	}

	//---------------------------------------------------------------------------
	//Desc:    converts String to int
	//Params:  input - String to convert
	//Returns: converted to int
	//---------------------------------------------------------------------------
	int StringHelper::toInt(const String &input) {
		return (_ttoi(input.c_str()));
	}

	//---------------------------------------------------------------------------
	//Desc:    converts String to float
	//Params:  input - String to convert
	//Returns: converted to float
	//---------------------------------------------------------------------------
	float StringHelper::toFloat(const String &input) {
		return (float)(_tstof(input.c_str()));
	}

	//---------------------------------------------------------------------------
	//Desc:    converts String to double
	//Params:  input - String to convert
	//Returns: converted to double
	//---------------------------------------------------------------------------
	double StringHelper::toDouble(const String &input) {
		return (double)(_tstof(input.c_str()));
	}

	//---------------------------------------------------------------------------
	//Desc:    gets word and converts it to int
	//Params:  input - String containing word to convert, n - word number
	//Returns: converted to int
	//---------------------------------------------------------------------------
	int StringHelper::getInt(const String &input, unsigned int n) {
		String temp = getWord(input, n);
		return toInt(temp);
	}

	//---------------------------------------------------------------------------
	//Desc:    gets word and converts it to float
	//Params:  input - String containing word to convert, n - word number
	//Returns: converted to float
	//---------------------------------------------------------------------------
	float StringHelper::getFloat(const String &input, unsigned int n) {
		String temp = getWord(input, n);
		return toFloat(temp);
	}

	//---------------------------------------------------------------------------
	//Desc:    gets word and converts it to double
	//Params:  input - String containing word to convert, n - word number
	//Returns: converted to double
	//---------------------------------------------------------------------------
	double StringHelper::getDouble(const String &input, unsigned int n) {
		String temp = getWord(input, n);
		return toDouble(temp);
	}

	//---------------------------------------------------------------------------
	//Desc:    converts String to upper case
	//Params:  input - String to convert
	//Returns: String in upper case
	//---------------------------------------------------------------------------
	String StringHelper::toUpper(const String &input) {
		String buf = input;
		strupr((char *)buf.c_str());
		return buf;
	}

	//---------------------------------------------------------------------------
	//Desc:    converts int to String
	//Params:  i - int to convert
	//Returns: String from int
	//---------------------------------------------------------------------------
	String StringHelper::fromInt(int i) {
		TCHAR buf[32];
		_stprintf(buf, _T("%d"), i);
		return String(buf);
	}

	//---------------------------------------------------------------------------
	//Desc:    converts float to String
	//Params:  i - float to convert
	//Returns: String from float
	//---------------------------------------------------------------------------
	String StringHelper::fromFloat(float i) {
		TCHAR buf[32];
		_stprintf(buf, _T("%f"), i);
		return String(buf);
	}

	//---------------------------------------------------------------------------
	//Desc:    converts double to String
	//Params:  i - double to convert
	//Returns: String from double
	//---------------------------------------------------------------------------
	String StringHelper::fromDouble(double i) {
		TCHAR buf[32];
		_stprintf(buf, _T("%f"), i);
		return String(buf);
	}

	//---------------------------------------------------------------------------
	//Desc:    converts bool to String
	//Params:  i - bool to convert
	//Returns: String from bool
	//---------------------------------------------------------------------------
	String StringHelper::fromBool(bool i) {
		if (i) {
			return _T("TRUE");
		}
		else {
			return _T("FALSE");
		}
	}

	String StringHelper::printf(const TCHAR *format, ...) {
		va_list arg;
		va_start(arg, format);

		static TCHAR buf[32];
		_vsntprintf(buf, sizeof(buf), format, arg);

		va_end(arg);

		return String(buf);
	}
}