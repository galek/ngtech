/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaevgmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

//**************************************
#include "String.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
//**************************************
/*
*/
String::String()
{
}

/*
*/
String::String(const char *str)
{
	assign(str);
}

/*
*/
String::String(const std::string &str)
{
	assign(str);
}

/*
*/
String::String(int i)
{
	char str[1024];
	sprintf(str, "%i", i);

	assign(str);
}

/*
*/
String::String(unsigned int i)
{
	char str[1024];
	sprintf(str, "%u", i);

	assign(str);
}

/*
*/
String::String(float i)
{
	char str[1024];
	sprintf(str, "%f", i);

	assign(str);
}

/*
*/
String::String(double i)
{
	char str[1024];
	sprintf(str, "%lf", i);

	assign(str);
}

/*
*/
String::String(bool i)
{
	char str[1024];
	sprintf(str, "%i", i);

	assign(str);
}

/*
*/
String String::GetWord(int n)
{
	String output;
	int k = 0;
	int pos = 0, last = 0;

	while (pos < length())
	{
		last = pos;
		pos = find_first_of(' ', pos);

		k++;

		if (k == n)
		{
			output = substr(last, pos - last);
			return output;
		}

		if (pos < length())
		{
			pos = find_first_not_of(' ', pos);
		}
	}
	return "";
}

/*
*/
String String::GetQuotedWord(int n)
{
	String output;
	int k = 0;
	int pos = 0, last = 0;

	while (pos < length())
	{
		last = pos;
		pos = find_first_of(' ', pos);

		k++;

		if (k == n)
		{
			if (data()[last] == '\"')
			{
				pos = find_first_of('\"', last+1);
				output = substr(last + 1, pos - last - 1);
			} 
			else
			{
				output = substr(last, pos - last);
			}
			return output;
		}

		if (pos < length())
		{
			pos = find_first_not_of(' ', pos);
		}
	}
	return "";
}

/*
*/
int String::GetWordCount()
{
	int n = 0;
	int pos = 0;

	if (data()[pos] == ' ')
	{
		pos = find_first_not_of(' ', pos);
	}

	while (pos < length())
	{
		n++;
		pos = find_first_of(' ', pos);
		if (pos < length())
		{
			pos = find_first_not_of(' ',pos);
		}
	}
	return n;
}

String String::CutFileExt()
{
	if (size() == 0)
		return "";

	String buf;

	int i = 0;
	while ((data()[i] != '.') && (i < (int)size()))
	{
		buf += data()[i];
		i++;
	}
	return buf;
}

String String::GetFileExt()
{
	if (size() == 0)
		return "";

	int i = (int)size() - 1;
	String buf;

	while ((data()[i] != '.') && (i > 0))
	{
		buf = data()[i] + buf;
		i--;
	}
	return buf;
}

/*
*/
int String::ToInt()
{
	return atoi(c_str());
}

/*
*/
float String::ToFloat()
{
	return atof(c_str());
}

/*
*/
double String::ToDouble()
{
	return atof(c_str());
}

/*
*/
bool String::ToBool()
{
	return (atoi(c_str()) != 0);
}

/*
*/
bool String::Strstr(const String &line)
{
	return (strstr(c_str(), line.c_str()) != 0);
}

/*
*/
void String::Printf(const String &format, ...)
{
	char cformat[1024];
	strcpy(cformat, format);

	va_list arg;
	va_start(arg, cformat);
	
	char buf[1024];
	vsnprintf(buf, sizeof(buf), cformat, arg);

	va_end(arg);

	assign(buf);
}

/*
*/
String::operator const char*() const
{
	return c_str();
}