/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaevgmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
#include <string>
//**************************************

/**
std::string
*/
class String : public std::string
{
public:
	/**
	String constructor
	*/
	String();

	/**
	String constructor
	*/
	String(const char *str);

	/**
	String constructor
	*/
	String(const std::string &str);

	/**
	String constructor
	*/
	String(int i);

	/*
	String constructor
	*/
	String(unsigned int i);

	/**
	String constructor
	*/
	String(float i);

	/**
	String constructor
	*/
	String(double i);

	/**
	String constructor
	*/
	String(bool i);

	/**
	Gets word from String. Words are separated by space
	*/
	String GetWord(int n);

	/**
	Gets quoted word from String. Words are separated by space and quoted word can contain spaces
	*/
	String GetQuotedWord(int n);

	/**
	Gets words count. Words are separated by space
	*/
	int GetWordCount();

	/**
	Cuts file ext from file name
	*/
	String CutFileExt();

	/**
	Gets file ext from file name
	*/
	String GetFileExt();

	/**
	Converts String to int
	*/
	int ToInt();

	/**
	Converts String to float
	*/
	float ToFloat();

	/**
	Converts String to double
	*/
	double ToDouble();

	/**
	Converts String to bool
	*/
	bool ToBool();

	/**
	strstr analog
	*/
	bool Strstr(const String &line);

	/**
	sprintf analog
	*/
	void Printf(const String &format,...);

	/**
	Converts String to const char *
	*/
	operator const char*() const;
};