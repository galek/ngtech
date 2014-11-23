//**************************************
#include "StringHelper.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
#include <algorithm>
//**************************************

namespace NGTech {

	/*
	*/
	int StringHelper::getWordsNumber(const String &input) {
		int n = 0;
		size_t pos = 0;

		if(input[pos] == ' ') pos = input.find_first_not_of(' ',pos);
		while(pos<input.length()) {
			n++;
			pos = input.find_first_of(' ',pos);
			if(pos < input.length()) pos=input.find_first_not_of(' ',pos);
		}
		return n;
	}

	/*
	*/
	String StringHelper::getWord(const String &input, unsigned int n) {
		String output;
		unsigned int k = 0;
		size_t pos = 0, last = 0;

		if((input[pos] == ' ') || (input[pos] == '\x9')) {
			pos = output.find_first_not_of(" \x9",pos);
		}

		while(pos < input.length()) {
			k++;
			last = pos;
			pos = input.find_first_of(' ',pos);
			if(k == n) {
				output = input.substr(last, pos - last);
				return output;
			}
			if(pos < input.length()) {
				pos = input.find_first_not_of(' ', pos);
			}
		}
		return "";
	}

	/*
	*/
	int StringHelper::toInt(const String &input) {
		return (atoi(input.c_str()));
	}

	/*
	*/
	float StringHelper::toFloat(const String &input) {
		return (float)(atof(input.c_str()));
	}

	/*
	*/
	double StringHelper::toDouble(const String &input) {
		return (double)(atof(input.c_str()));
	}

	/*
	*/
	int StringHelper::getInt(const String &input, unsigned int n) {
		String temp = getWord(input, n);
		return toInt(temp);
	}

	/*
	*/
	float StringHelper::getFloat(const String &input, unsigned int n) {
		String temp = getWord(input, n);
		return toFloat(temp);
	}

	/*
	*/
	double StringHelper::getDouble(const String &input, unsigned int n) {
		String temp = getWord(input, n);
		return toDouble(temp);
	}

	/*
	*/
	String StringHelper::toUpper(const String &input) {
		String buf = input;
#ifdef __ANDROID__
		std::transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
#else
		_strupr((char *)buf.c_str());
#endif
		return buf;
	}

	/*
	*/
	String StringHelper::fromInt(int i) {
		char buf[32];
		sprintf(buf, "%d", i);
		return String(buf);
	}

	/*
	*/
	String StringHelper::fromFloat(float i) {
		char buf[32];
		sprintf(buf, "%f", i);
		return String(buf);
	}

	/*
	*/
	String StringHelper::fromDouble(double i) {
		char buf[32];
		sprintf(buf, "%f", i);
		return String(buf);
	}

	/*
	*/
	String StringHelper::fromBool(bool i) {
		if(i) {
			return "TRUE"; 
		} else {
			return "FALSE";
		}
	}
}